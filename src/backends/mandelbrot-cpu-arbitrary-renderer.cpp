#include "mandelbrot-cpu-arbitrary-renderer.h"
#include "vecmath.h"
#include "colored-vertex.h"
#include "mandelbrot-shader-loader.h"

#include <mpfr.h>
#include <imgui.h>
#include <cmath>

const char* mandelbrot_cpu_arbitrary_renderer::get_backend_name() {
    return "MANDELBROT CPU ARBITRARY PRECISION";
}

// Compute MPFR bits needed to distinguish adjacent pixels at this zoom.
static int compute_precision(double zoom, int width) {
    double bits = -std::log2(std::max(zoom, 1e-300)) + std::log2(std::max(width, 1)) + 64.0;
    int ibits = std::max(64, (int)std::ceil(bits));
    return ((ibits + 63) / 64) * 64;  // round up to multiple of 64
}

// Per-thread MPFR variable workspace. cr and ci must be set before calling iterate().
struct mpfr_workspace {
    mpfr_t zr, zi, zr2, zi2, tmp, len2;
    mpfr_t cr, ci;
    mpfr_t threshold;

    mpfr_workspace(mpfr_prec_t prec) {
        mpfr_init2(zr,  prec); mpfr_init2(zi,  prec);
        mpfr_init2(zr2, prec); mpfr_init2(zi2, prec);
        mpfr_init2(tmp, prec); mpfr_init2(len2, prec);
        mpfr_init2(cr,  prec); mpfr_init2(ci,  prec);
        mpfr_init2(threshold, prec);
        mpfr_set_d(threshold, 16.0, MPFR_RNDN);
    }

    ~mpfr_workspace() {
        mpfr_clear(zr);  mpfr_clear(zi);
        mpfr_clear(zr2); mpfr_clear(zi2);
        mpfr_clear(tmp); mpfr_clear(len2);
        mpfr_clear(cr);  mpfr_clear(ci);
        mpfr_clear(threshold);
    }
};

static math::vec<double, 3> iterate(mpfr_workspace& ws, int max_iter) {
    mpfr_set_d(ws.zr, 0.0, MPFR_RNDN);
    mpfr_set_d(ws.zi, 0.0, MPFR_RNDN);

    for (int i = 0; i < max_iter; ++i) {
        // tmp  = zr^2
        // zr2  = zi^2  (temporarily)
        // len2 = zr^2 + zi^2   (escape check on old zr, zi)
        // zr2  = zr^2 - zi^2 + cr
        // zi2  = 2*zr*zi + ci

        mpfr_mul(ws.tmp, ws.zr, ws.zr, MPFR_RNDN);    // tmp  = zr^2
        mpfr_mul(ws.zr2, ws.zi, ws.zi, MPFR_RNDN);    // zr2  = zi^2

        mpfr_add(ws.len2, ws.tmp, ws.zr2, MPFR_RNDN); // len2 = zr^2 + zi^2

        mpfr_sub(ws.zr2, ws.tmp, ws.zr2, MPFR_RNDN);  // zr2  = zr^2 - zi^2
        mpfr_add(ws.zr2, ws.zr2, ws.cr,  MPFR_RNDN);  // zr2 += cr

        mpfr_mul(ws.zi2, ws.zr, ws.zi,   MPFR_RNDN);  // zi2  = zr*zi
        mpfr_mul_2ui(ws.zi2, ws.zi2, 1,  MPFR_RNDN);  // zi2  = 2*zr*zi
        mpfr_add(ws.zi2, ws.zi2, ws.ci,  MPFR_RNDN);  // zi2 += ci

        mpfr_set(ws.zr, ws.zr2, MPFR_RNDN);
        mpfr_set(ws.zi, ws.zi2, MPFR_RNDN);

        if (mpfr_greater_p(ws.len2, ws.threshold)) {
            return math::vec((double)(sin(0.5 * i) + 0.4),
                             (double)(sin(i + 5.0) + 0.4),
                             (double)(cos(i * 0.4) + 0.4));
        }
    }

    return math::vec(0.0, 0.0, 0.0);
}

void mandelbrot_cpu_arbitrary_renderer::worker_body() {
    int last_phase = -1;

    // Workspace is kept alive across frames; recreated only when precision changes.
    std::unique_ptr<mpfr_workspace> ws;
    int ws_prec = 0;

    // Per-row MPFR temporaries, same lifetime as the workspace.
    mpfr_t cx, cy, cx_step;
    bool coords_init = false;

    auto init_coords = [&](int prec) {
        if (coords_init) {
            mpfr_clear(cx); mpfr_clear(cy); mpfr_clear(cx_step);
        }
        mpfr_init2(cx,      prec);
        mpfr_init2(cy,      prec);
        mpfr_init2(cx_step, prec);
        coords_init = true;
    };

    while (true) {
        // Sleep until a new frame is posted or stop is requested.
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [&]{ return m_stop || m_phase != last_phase; });
            if (m_stop) break;
            last_phase = m_phase;
        }

        // Snapshot frame parameters (safe: happens-after the mutex release above).
        const int    width    = m_frame_width;
        const int    height   = m_frame_height;
        const int    prec     = m_frame_prec;
        const double zoom     = m_frame_zoom;
        const double pos_x    = m_frame_pos_x;
        const double pos_y    = m_frame_pos_y;
        const int    max_iter = max_iterations;

        // Recreate workspace and coord variables only when precision changes.
        if (!ws || ws_prec != prec) {
            ws = std::make_unique<mpfr_workspace>((mpfr_prec_t)prec);
            ws_prec = prec;
            init_coords(prec);
        }

        const double aspect   = (double)width / height;
        const double scale_x  = 2.0 * aspect * zoom / width;
        const double offset_x = -aspect * zoom + pos_x;
        const double scale_y  = 2.0 * zoom / height;
        const double offset_y = -zoom + pos_y;

        mpfr_set_d(cx_step, scale_x, MPFR_RNDN);

        int row;
        while ((row = m_next_row.fetch_add(1)) < height) {
            // cy = row * scale_y + offset_y
            mpfr_set_d(cy, scale_y,      MPFR_RNDN);
            mpfr_mul_si(cy, cy, (long)row, MPFR_RNDN);
            mpfr_add_d(cy, cy, offset_y,  MPFR_RNDN);
            mpfr_set(ws->ci, cy, MPFR_RNDN);

            // cx starts at col=0: offset_x + 0*scale_x
            mpfr_set_d(cx, offset_x, MPFR_RNDN);

            for (int col = 0; col < width; ++col) {
                mpfr_set(ws->cr, cx, MPFR_RNDN);

                colored_vertex& v = points[row * width + col];
                v.point = math::vec(col * 2.0 / width  - 1.0,
                                    row * 2.0 / height - 1.0);
                v.color = iterate(*ws, max_iter);

                mpfr_add(cx, cx, cx_step, MPFR_RNDN);  // increment by scale_x
            }
        }

        // Signal completion. Release ordering ensures all pixel writes above
        // are visible to the main thread when it loads m_threads_done.
        m_threads_done.fetch_add(1, std::memory_order_release);
    }

    if (coords_init) {
        mpfr_clear(cx); mpfr_clear(cy); mpfr_clear(cx_step);
    }
}

void mandelbrot_cpu_arbitrary_renderer::start_frame(math::vec<double, 2> position, double zoom) {
    // Write frame parameters before the mutex so workers see them after waking
    // (the mutex acquire in wait() provides the happens-before edge).
    m_frame_prec   = compute_precision(zoom, get_width());
    m_frame_width  = (int)get_width();
    m_frame_height = (int)get_height();
    m_frame_zoom   = zoom;
    m_frame_pos_x  = position.x();
    m_frame_pos_y  = position.y();
    current_precision = m_frame_prec;

    m_next_row.store(0, std::memory_order_relaxed);
    m_threads_done.store(0, std::memory_order_relaxed);

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_computing = true;
        ++m_phase;
        m_cv.notify_all();
    }
}

void mandelbrot_cpu_arbitrary_renderer::setup() {
    points.set_layout(math::vector_layout<double, 2>() +
                      math::vector_layout<double, 3>());
    points.resize(get_width() * get_height());

    load_mandelbrot_shader(colored_renderer, "gradient");

    const int num_threads = (int)std::thread::hardware_concurrency();
    m_threads.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i)
        m_threads.emplace_back(&mandelbrot_cpu_arbitrary_renderer::worker_body, this);
}

void mandelbrot_cpu_arbitrary_renderer::draw() {
    // Non-blocking check: did the background computation finish?
    if (m_computing &&
        m_threads_done.load(std::memory_order_acquire) == (int)m_threads.size()) {
        m_computing = false;
        points.update();  // safe: all workers are done, no one writes to points
    }

    // Always draw whatever is currently in the GPU buffer (last complete frame).
    gl::draw(gl::drawing_type::POINTS, points, colored_renderer);

    // Kick off the next frame unless one is already in flight.
    if (!m_computing)
        start_frame(position, zoom);
}

void mandelbrot_cpu_arbitrary_renderer::additional_ui() {
    ImGui::SliderInt("Max Iterations", &max_iterations, 64, 10000);
    ImGui::Text("Precision: %d bits", current_precision);
    ImGui::Text("Threads: %d", (int)m_threads.size());
}

mandelbrot_cpu_arbitrary_renderer::~mandelbrot_cpu_arbitrary_renderer() {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stop = true;
        m_cv.notify_all();
    }
    for (auto& t : m_threads)
        t.join();
}
