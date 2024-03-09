#pragma once

#include "opengl-setup.h"


inline void load_mandelbrot_shader(gl::shaders::shader_program &shader_program, std::string name) {
    const char* mandelbrot_shade_home = std::getenv("MANDELBROT_SHADER_HOME");
    if (!mandelbrot_shade_home)
        mandelbrot_shade_home = "res/";

    shader_program.from_file(std::string { mandelbrot_shade_home } + "/" + name + ".glsl");
}
