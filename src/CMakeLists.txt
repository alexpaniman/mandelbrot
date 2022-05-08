add_executable(mandelbrot mandelbrot.cpp
    mandelbrot-gpu-renderer.cpp
    mandelbrot-gpu-single-precision-renderer.cpp
    mandelbrot-cpu-unoptimized-renderer.cpp
    mandelbrot-cpu-vectorized-renderer.cpp
    mandelbrot-cpu-optimized-renderer.cpp)

target_include_directories(mandelbrot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(mandelbrot PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})

target_link_libraries(mandelbrot gl-imgui)
