#include "opengl-setup.h"
#include "math.h"
#include "vertex-array.h"
#include <GLFW/glfw3.h>

#include <fstream>
#include <initializer_list>
#include <limits>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cassert>


namespace gl {

    // -------------------------------- SHADERS GENERAL --------------------------------

    std::map<const std::string, const shaders::shader_type> shaders::shader_names = {
        std::pair(  "vertex", shaders::shader_type::VERTEX),
        std::pair("fragment", shaders::shader_type::FRAGMENT)
    };

    shaders::compiled_shader shaders::compile_shader(const shaders::raw_shader& shader_to_compile) {
        const shaders::shader_type type = shader_names[shader_to_compile.type];

        unsigned int id = glCreateShader((unsigned int) type);

        const char* src = shader_to_compile.source_code.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE) {
            int length = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

            assert(length > 0 && "Length should be > 0");

            char* message = new char[(unsigned int) length];
            glGetShaderInfoLog(id, length, &length, message);

            std::string error_message = "Failed to compile shader!\n";
            error_message += "==>    type: " + shader_to_compile.type;
            error_message += "==> message: " + std::string(message);

            // Because /message/ was already copied to /error_message/
            delete[] message;

            throw std::runtime_error(error_message);
        }

        return shaders::compiled_shader { type, id };
    }

    std::vector<shaders::compiled_shader>
    shaders::compile_shaders(const std::vector<shaders::raw_shader> raw_shaders) {

        std::vector<shaders::compiled_shader> compiled_shaders;
        compiled_shaders.reserve(raw_shaders.size());

        for (auto raw_shader: raw_shaders)
            compiled_shaders.push_back(compile_shader(raw_shader));

        return compiled_shaders;
    }

    std::vector<shaders::raw_shader> shaders::extract_shaders(const std::string filename) {
        std::ifstream input_file(filename);

        std::vector<shaders::raw_shader> shaders;

        std::string line;
        while (std::getline(input_file, line)) {
            if (line.find("#shader") == std::string::npos) {
                if (shaders.empty())
                    continue;

                shaders.back().source_code.append(line + "\n");
            } else {
                std::istringstream words(line);

                // Skip our directive "#shader":
                words.ignore(std::numeric_limits<std::streamsize>::max(), ' ');

                std::string shader_type_name; 
                std::getline(words, shader_type_name, ' ');

                shaders.push_back({ shader_type_name, {} });
            }
        }

        return shaders;
    }

    // --------------------------------- SHADER PROGRAM --------------------------------

    shaders::shader_program::shader_program(): id(glCreateProgram()) {}

    shaders::shader_program::shader_program(const std::string filename): shader_program() {
        from_file(filename);
    }

    void shaders::shader_program::from_shaders(const std::vector<shaders::compiled_shader> raw_shaders) {
        for (auto shader: raw_shaders)
            glAttachShader(id, shader.id);

        glLinkProgram(id);
        glValidateProgram(id);

        for (auto shader: raw_shaders)
            glDeleteProgram(shader.id);
    };

    void shaders::shader_program::from_shaders(const std::vector<shaders::raw_shader> raw_shaders) {
        std::vector<shaders::compiled_shader> compiled_shaders =
            shaders::compile_shaders(raw_shaders);

        from_shaders(compiled_shaders);
    }


    void shaders::shader_program::from_file(const std::string filename) {
        return from_shaders(shaders::extract_shaders(filename));
    }

    void shaders::shader_program::bind() {
        glUseProgram(id);
    }

    shaders::shader_program::~shader_program() {
        glDeleteProgram(id);
    }

    // ---------------------------------- GLFW WINDOW ----------------------------------

    window::window(const int width, const int height, const char* title):
            width(width), height(height) {

        if (!glfwInit())
            throw std::runtime_error("Failed to initialize glfw!");

        glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);

        if (glfw_window == NULL) {
            glfwTerminate();

            const char* error_message = NULL;
            glfwGetError(&error_message);

            throw std::runtime_error("Failed to open window!\n" +
                                     std::string(error_message));
        }

        bind();

        if (glewInit() != GLEW_OK)
            throw std::runtime_error("Failed to initialize glew!");
    }
    
    void window::bind() {
        glfwMakeContextCurrent(glfw_window);
    }


    int window::get_fps() const noexcept {
        return current_fps;
    }

    void window::draw_loop() {
        setup();

        double last_time = glfwGetTime();
        while (!glfwWindowShouldClose(glfw_window)) {
            glClear(GL_COLOR_BUFFER_BIT);

            draw();

            glfwSwapBuffers(glfw_window);
            glfwPollEvents();

            double current_time = glfwGetTime();
            current_fps ++;
            if (current_time - last_time >= 1.0) {
                current_fps = 0;
                last_time += 1.0;
            }
        }
    }

    window::~window() {
        glfwTerminate();
    }

    // --------------------------------- POINTS BUFFER ---------------------------------

    static unsigned int generate_buffer() {
        unsigned int generated_buffer_id = 0;
        glGenBuffers(1 /* single buffer */, &generated_buffer_id);

        return generated_buffer_id;
    }

    points_buffer::points_buffer(): id(generate_buffer()) {}

    void points_buffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void points_buffer::assign(const std::initializer_list<math::vec<double, 2>> points_initializer) {
        this->points.assign(points_initializer);
        upload_data();
    }

    void points_buffer::upload_data() {
        bind();
        glBufferData(GL_ARRAY_BUFFER, points.size() * 2 * sizeof(double),
                     &(*points.begin()), GL_STATIC_DRAW);

        const unsigned int vec2_attrib_index = 0;

        glEnableVertexAttribArray(vec2_attrib_index);
        glVertexAttribPointer(vec2_attrib_index, 2, GL_DOUBLE,
                              GL_FALSE, sizeof(double) * 2, 0);
    }

    size_t points_buffer::size() const noexcept {
        return points.size();
    }

    math::vec<double, 2>& points_buffer::operator[](const size_t index) {
        return points[index];
    }

    // ------------------------------------ DRAWING ------------------------------------

    void draw_buffer(const drawing_type type, const vertex_array& buffer_to_draw) {
        buffer_to_draw.bind();
        glDrawArrays((unsigned int) type, 0, (int) 2);
    }
}
