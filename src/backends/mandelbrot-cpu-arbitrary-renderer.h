#pragma once

#include "opengl-setup.h"
#include "mandelbrot-renderer.h"
#include "colored-vertex.h"
#include "renderer.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

class mandelbrot_cpu_arbitrary_renderer: public mandelbrot_renderer {
private:
    gl::vertex_vector_array<colored_vertex> points;
    gl::shaders::shader_program colored_renderer;

    int max_iterations = 512;
    int current_precision = 64;

    // Persistent thread pool
    std::vector<std::thread> m_threads;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    int m_phase = 0;           // incremented each frame to wake workers
    int m_threads_done = 0;    // how many workers finished this frame
    bool m_stop = false;

    // Per-frame parameters written by main thread before notify,
    // read by workers after waking (ordering guaranteed by the mutex).
    std::atomic<int> m_next_row{0};
    int    m_frame_width  = 0;
    int    m_frame_height = 0;
    int    m_frame_prec   = 64;
    double m_frame_zoom   = 1.0;
    double m_frame_pos_x  = 0.0;
    double m_frame_pos_y  = 0.0;

    void worker_body();
    void draw_mandelbrot(math::vec<double, 2> position, double zoom);

public:
    using mandelbrot_renderer::mandelbrot_renderer;

    void setup() override;
    void draw()  override;

    void additional_ui() override;

    const char* get_backend_name() override;

    ~mandelbrot_cpu_arbitrary_renderer();
};
