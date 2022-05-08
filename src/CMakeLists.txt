add_executable(mandelbrot mandelbrot.cpp
    backends/mandelbrot-gpu-renderer.cpp
    backends/mandelbrot-gpu-single-precision-renderer.cpp
    backends/mandelbrot-cpu-unoptimized-renderer.cpp
    backends/mandelbrot-cpu-vectorized-renderer.cpp
    backends/mandelbrot-cpu-optimized-renderer.cpp)

target_include_directories(mandelbrot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends)

set_target_properties(mandelbrot PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})

target_link_libraries(mandelbrot gl-imgui)
