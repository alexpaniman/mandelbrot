#include "mandelbrot-cpu-arbitrary-renderer.h"
#include "vecmath.h"
#include "colored-vertex.h"
#include "mandelbrot-shader-loader.h"

#include <mpfr.h>
#include <imgui.h>
#include <cmath>
#include <atomic>
#include <thread>
#include <vector>

const char* mandelbrot_cpu_arbitrary_renderer::get_backend_name() {
    return "MANDELBROT CPU ARBITRARY PRECISION";
}

// Compute the number of MPFR bits needed to distinguish adjacent pixels at this zoom.
// Adjacent pixels differ by zoom * 2 / width in the complex plane.
// We need -log2(zoom * 2 / width) + 64 guard bits.
static int compute_precision(double zoom, int width) {
    double bits = -std::log2(std::max(zoom, 1e-300)) + std::log2(std::max(width, 1)) + 64.0;
    // Round up to next multiple of 64 for MPFR efficiency
    int ibits = std::max(64, (int)std::ceil(bits));
    return ((ibits + 63) / 64) * 64;
}

// Per-thread MPFR variable workspace. All variables share the same precision.
// cr and ci must be set before calling iterate().
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
        // len2 = zr^2 + zi^2   (escape check uses old zr, zi)
        // zr2  = zr^2 - zi^2 + cr   (new real part)
        // zi2  = 2*zr*zi + ci        (new imaginary part)

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

void mandelbrot_cpu_arbitrary_renderer::draw_mandelbrot(math::vec<double, 2> position, double zoom) {
    const int width  = (int)get_width();
    const int height = (int)get_height();

    const int prec = compute_precision(zoom, width);
    current_precision = prec;

    const double pos_x  = position.x();
    const double pos_y  = position.y();
    const double aspect = (double)width / height;

    // Mapping from pixel (col, row) to Mandelbrot coordinate:
    //   cx = col * (2 * aspect * zoom / width)  + (-aspect * zoom + pos_x)
    //   cy = row * (2 * zoom / height)           + (-zoom + pos_y)
    const double scale_x  = 2.0 * aspect * zoom / width;
    const double offset_x = -aspect * zoom + pos_x;
    const double scale_y  = 2.0 * zoom / height;
    const double offset_y = -zoom + pos_y;

    const int num_threads = (int)std::thread::hardware_concurrency();
    std::atomic<int> next_row(0);

    auto worker = [&]() {
        mpfr_workspace ws((mpfr_prec_t)prec);

        // Per-thread MPFR temporaries for coordinate computation
        mpfr_t cx, cy, cx_step, cy_base;
        mpfr_init2(cx,      prec);
        mpfr_init2(cy,      prec);
        mpfr_init2(cx_step, prec);
        mpfr_init2(cy_base, prec);

        mpfr_set_d(cx_step, scale_x,  MPFR_RNDN);

        int row;
        while ((row = next_row.fetch_add(1)) < height) {
            // cy = row * scale_y + offset_y
            mpfr_set_d(cy, scale_y, MPFR_RNDN);
            mpfr_mul_si(cy, cy, (long)row, MPFR_RNDN);
            mpfr_add_d(cy, cy, offset_y, MPFR_RNDN);
            mpfr_set(ws.ci, cy, MPFR_RNDN);

            // cx starts at col=0: cx = 0 * scale_x + offset_x = offset_x
            mpfr_set_d(cx, offset_x, MPFR_RNDN);

            for (int col = 0; col < width; ++col) {
                mpfr_set(ws.cr, cx, MPFR_RNDN);

                colored_vertex& v = points[row * width + col];
                v.point = math::vec(col * 2.0 / width  - 1.0,
                                    row * 2.0 / height - 1.0);
                v.color = iterate(ws, max_iterations);

                // Advance cx by scale_x (incremental — avoids mul_si per column)
                mpfr_add(cx, cx, cx_step, MPFR_RNDN);
            }
        }

        mpfr_clear(cx); mpfr_clear(cy);
        mpfr_clear(cx_step); mpfr_clear(cy_base);
    };

    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    for (int t = 0; t < num_threads; ++t)
        threads.emplace_back(worker);
    for (auto& t : threads)
        t.join();

    points.update();
}

void mandelbrot_cpu_arbitrary_renderer::setup() {
    points.set_layout(math::vector_layout<double, 2>() +
                      math::vector_layout<double, 3>());

    points.resize(get_width() * get_height());

    load_mandelbrot_shader(colored_renderer, "gradient");
}

void mandelbrot_cpu_arbitrary_renderer::draw() {
    draw_mandelbrot(position, zoom);
    gl::draw(gl::drawing_type::POINTS, points, colored_renderer);
}

void mandelbrot_cpu_arbitrary_renderer::additional_ui() {
    ImGui::SliderInt("Max Iterations", &max_iterations, 64, 10000);
    ImGui::Text("Precision: %d bits", current_precision);
    ImGui::Text("Threads: %d", (int)std::thread::hardware_concurrency());
}
