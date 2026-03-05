set(MANDELBROT_SOURCES
    mandelbrot.cpp
    backends/mandelbrot-gpu-renderer.cpp
    backends/mandelbrot-gpu-single-precision-renderer.cpp
    backends/mandelbrot-cpu-unoptimized-renderer.cpp
    backends/mandelbrot-cpu-vectorized-renderer.cpp
    backends/mandelbrot-cpu-arbitrary-renderer.cpp)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|i686")
    list(APPEND MANDELBROT_SOURCES backends/mandelbrot-cpu-optimized-renderer.cpp)
endif()

add_executable(mandelbrot ${MANDELBROT_SOURCES})

target_include_directories(mandelbrot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends)

set_target_properties(mandelbrot PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})

find_library(MPFR_LIBRARY NAMES mpfr REQUIRED)
find_path(MPFR_INCLUDE_DIR NAMES mpfr.h REQUIRED)

target_include_directories(mandelbrot PRIVATE ${MPFR_INCLUDE_DIR})
target_link_libraries(mandelbrot gl-imgui ${MPFR_LIBRARY})

install(TARGETS mandelbrot DESTINATION bin)
