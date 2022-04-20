#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <initializer_list>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

#include "math.h"
#include "vertex-array.h"

namespace gl::shaders {
    enum class shader_type {
        VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER
    };

    extern std::map<const std::string, const shader_type> shader_names;


    struct raw_shader final {
        std::string type;
        std::string source_code;
    };

    struct compiled_shader final {
        const shader_type type;
        const unsigned int id;
    };

    std::vector<raw_shader> extract_shaders(std::string filename);

    compiled_shader compile_shader(const raw_shader& shader_to_compile);

    std::vector<compiled_shader>
        compile_shaders(std::vector<raw_shader> raw_shaders);


    class shader_program final {
    private:
        const unsigned int id;

    public:
        shader_program();
        shader_program(std::string filename);

        void bind();

        void from_shaders(std::vector<compiled_shader> shaders);
        void from_shaders(std::vector<raw_shader>      shaders);

        void from_file(std::string filename);

        ~shader_program();
    };
}

namespace gl {
    class window {
    private:
        GLFWwindow* glfw_window;
        int current_fps;

    protected:
        const int width, height;

    public:
        window(int width, int height, const char* title);

        // This class shouldn't be copied or moved
        window(const window&) = delete;
        window& operator=(const window&) = delete;

        void bind();

        int get_fps() const noexcept;
        void draw_loop();

        virtual void setup() = 0;
        virtual void draw()  = 0;

        virtual ~window();
    };


    class points_buffer final {
    private:
        std::vector<math::vec<double, 2>> points;

    public:
        const unsigned int id;

        points_buffer();

        void resize(size_t buffer_size);
        void assign(std::initializer_list<math::vec<double, 2>> points);

        size_t size() const noexcept;

        void bind();
        void upload_data();

        math::vec<double, 2>& operator[](size_t index);
    };

    enum class drawing_type {
        POINTS                   = GL_POINTS                  ,
        LINE_STRIP               = GL_LINE_STRIP              ,
        LINE_LOOP                = GL_LINE_LOOP               ,
        LINES                    = GL_LINES                   ,
        LINE_STRIP_ADJACENCY     = GL_LINE_STRIP_ADJACENCY    ,
        LINES_ADJACENCY          = GL_LINES_ADJACENCY         ,
        TRIANGLE_STRIP           = GL_TRIANGLE_STRIP          ,
        TRIANGLE_FAN             = GL_TRIANGLE_FAN            ,
        TRIANGLES                = GL_TRIANGLES               ,
        TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY      = GL_TRIANGLES_ADJACENCY     ,
        PATCHES                  = GL_PATCHES
    };

    void draw_buffer(drawing_type type, const vertex_array& buffer_to_draw);
}
