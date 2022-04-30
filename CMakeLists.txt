cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D _DEBUG -no-pie -ggdb3 -std=c++20 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -fPIE -O0")

set(CMAKE_CXX_LINKER_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D _DEBUG -no-pie -ggdb3 -std=c++20 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -fPIE -O0")

project(mandelbrot VERSION 1.0)

add_compile_definitions(GL_LOG_CALLS)

add_executable(mandelbrot mandelbrot.cpp opengl-setup.cpp vertex-array.cpp vertex-buffer.cpp vertex-layout.cpp math.cpp opengl-error-handler.cpp uniforms.cpp)

add_custom_command(
   TARGET mandelbrot PRE_BUILD
   COMMAND m4 opengl-wrapper.h.m4 > opengl-wrapper.h
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

target_include_directories(
    mandelbrot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})


set(OpenGL_GL_PREFERENCE GLVND)

find_package(GLEW   REQUIRED)
find_package(glfw3  REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT   REQUIRED)

target_include_directories(
    mandelbrot PUBLIC
    ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

target_link_libraries(
    mandelbrot PUBLIC
    ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} glfw)
