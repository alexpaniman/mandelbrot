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
#include "vertex-vector-array.h"

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

        void bind() const;
        unsigned int get_id() const;

        void from_shaders(std::vector<compiled_shader> shaders);
        void from_shaders(std::vector<raw_shader>      shaders);

        void from_file(std::string filename);

        ~shader_program();

    private:
        mutable std::map<std::string, int> uniform_locations;
        int get_uniform_location_cached(std::string uniform_name) const;

    public:
        template <typename uniform_type>
        void uniform(std::string name, uniform_type value) const;
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

        virtual void setup() {};
        virtual void draw()  = 0;

        virtual void on_fps_updated() {};

        enum class key {
            KEY_SPACE         = GLFW_KEY_SPACE,
            KEY_APOSTROPHE    = GLFW_KEY_APOSTROPHE,
            KEY_COMMA         = GLFW_KEY_COMMA,
            KEY_MINUS         = GLFW_KEY_MINUS,
            KEY_PERIOD        = GLFW_KEY_PERIOD,
            KEY_SLASH         = GLFW_KEY_SLASH,
            KEY_0             = GLFW_KEY_0,
            KEY_1             = GLFW_KEY_1,
            KEY_2             = GLFW_KEY_2,
            KEY_3             = GLFW_KEY_3,
            KEY_4             = GLFW_KEY_4,
            KEY_5             = GLFW_KEY_5,
            KEY_6             = GLFW_KEY_6,
            KEY_7             = GLFW_KEY_7,
            KEY_8             = GLFW_KEY_8,
            KEY_9             = GLFW_KEY_9,
            KEY_SEMICOLON     = GLFW_KEY_SEMICOLON,
            KEY_EQUAL         = GLFW_KEY_EQUAL,
            KEY_A             = GLFW_KEY_A,
            KEY_B             = GLFW_KEY_B,
            KEY_C             = GLFW_KEY_C,
            KEY_D             = GLFW_KEY_D,
            KEY_E             = GLFW_KEY_E,
            KEY_F             = GLFW_KEY_F,
            KEY_G             = GLFW_KEY_G,
            KEY_H             = GLFW_KEY_H,
            KEY_I             = GLFW_KEY_I,
            KEY_J             = GLFW_KEY_J,
            KEY_K             = GLFW_KEY_K,
            KEY_L             = GLFW_KEY_L,
            KEY_M             = GLFW_KEY_M,
            KEY_N             = GLFW_KEY_N,
            KEY_O             = GLFW_KEY_O,
            KEY_P             = GLFW_KEY_P,
            KEY_Q             = GLFW_KEY_Q,
            KEY_R             = GLFW_KEY_R,
            KEY_S             = GLFW_KEY_S,
            KEY_T             = GLFW_KEY_T,
            KEY_U             = GLFW_KEY_U,
            KEY_V             = GLFW_KEY_V,
            KEY_W             = GLFW_KEY_W,
            KEY_X             = GLFW_KEY_X,
            KEY_Y             = GLFW_KEY_Y,
            KEY_Z             = GLFW_KEY_Z,
            KEY_LEFT_BRACKET  = GLFW_KEY_LEFT_BRACKET,
            KEY_BACKSLASH     = GLFW_KEY_BACKSLASH,
            KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
            KEY_GRAVE_ACCENT  = GLFW_KEY_GRAVE_ACCENT,
            KEY_WORLD_1       = GLFW_KEY_WORLD_1,
            KEY_WORLD_2       = GLFW_KEY_WORLD_2,
            KEY_ESCAPE        = GLFW_KEY_ESCAPE,
            KEY_ENTER         = GLFW_KEY_ENTER,
            KEY_TAB           = GLFW_KEY_TAB,
            KEY_BACKSPACE     = GLFW_KEY_BACKSPACE,
            KEY_INSERT        = GLFW_KEY_INSERT,
            KEY_DELETE        = GLFW_KEY_DELETE,
            KEY_RIGHT         = GLFW_KEY_RIGHT,
            KEY_LEFT          = GLFW_KEY_LEFT,
            KEY_DOWN          = GLFW_KEY_DOWN,
            KEY_UP            = GLFW_KEY_UP,
            KEY_PAGE_UP       = GLFW_KEY_PAGE_UP,
            KEY_PAGE_DOWN     = GLFW_KEY_PAGE_DOWN,
            KEY_HOME          = GLFW_KEY_HOME,
            KEY_END           = GLFW_KEY_END,
            KEY_CAPS_LOCK     = GLFW_KEY_CAPS_LOCK,
            KEY_SCROLL_LOCK   = GLFW_KEY_SCROLL_LOCK,
            KEY_NUM_LOCK      = GLFW_KEY_NUM_LOCK,
            KEY_PRINT_SCREEN  = GLFW_KEY_PRINT_SCREEN,
            KEY_PAUSE         = GLFW_KEY_PAUSE,
            KEY_F1            = GLFW_KEY_F1,
            KEY_F2            = GLFW_KEY_F2,
            KEY_F3            = GLFW_KEY_F3,
            KEY_F4            = GLFW_KEY_F4,
            KEY_F5            = GLFW_KEY_F5,
            KEY_F6            = GLFW_KEY_F6,
            KEY_F7            = GLFW_KEY_F7,
            KEY_F8            = GLFW_KEY_F8,
            KEY_F9            = GLFW_KEY_F9,
            KEY_F10           = GLFW_KEY_F10,
            KEY_F11           = GLFW_KEY_F11,
            KEY_F12           = GLFW_KEY_F12,
            KEY_F13           = GLFW_KEY_F13,
            KEY_F14           = GLFW_KEY_F14,
            KEY_F15           = GLFW_KEY_F15,
            KEY_F16           = GLFW_KEY_F16,
            KEY_F17           = GLFW_KEY_F17,
            KEY_F18           = GLFW_KEY_F18,
            KEY_F19           = GLFW_KEY_F19,
            KEY_F20           = GLFW_KEY_F20,
            KEY_F21           = GLFW_KEY_F21,
            KEY_F22           = GLFW_KEY_F22,
            KEY_F23           = GLFW_KEY_F23,
            KEY_F24           = GLFW_KEY_F24,
            KEY_F25           = GLFW_KEY_F25,
            KEY_KP_0          = GLFW_KEY_KP_0,
            KEY_KP_1          = GLFW_KEY_KP_1,
            KEY_KP_2          = GLFW_KEY_KP_2,
            KEY_KP_3          = GLFW_KEY_KP_3,
            KEY_KP_4          = GLFW_KEY_KP_4,
            KEY_KP_5          = GLFW_KEY_KP_5,
            KEY_KP_6          = GLFW_KEY_KP_6,
            KEY_KP_7          = GLFW_KEY_KP_7,
            KEY_KP_8          = GLFW_KEY_KP_8,
            KEY_KP_9          = GLFW_KEY_KP_9,
            KEY_KP_DECIMAL    = GLFW_KEY_KP_DECIMAL,
            KEY_KP_DIVIDE     = GLFW_KEY_KP_DIVIDE,
            KEY_KP_MULTIPLY   = GLFW_KEY_KP_MULTIPLY,
            KEY_KP_SUBTRACT   = GLFW_KEY_KP_SUBTRACT,
            KEY_KP_ADD        = GLFW_KEY_KP_ADD,
            KEY_KP_ENTER      = GLFW_KEY_KP_ENTER,
            KEY_KP_EQUAL      = GLFW_KEY_KP_EQUAL,
            KEY_LEFT_SHIFT    = GLFW_KEY_LEFT_SHIFT,
            KEY_LEFT_CONTROL  = GLFW_KEY_LEFT_CONTROL,
            KEY_LEFT_ALT      = GLFW_KEY_LEFT_ALT,
            KEY_LEFT_SUPER    = GLFW_KEY_LEFT_SUPER,
            KEY_RIGHT_SHIFT   = GLFW_KEY_RIGHT_SHIFT,
            KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
            KEY_RIGHT_ALT     = GLFW_KEY_RIGHT_ALT,
            KEY_RIGHT_SUPER   = GLFW_KEY_RIGHT_SUPER,
            KEY_MENU          = GLFW_KEY_MENU,
        };

        virtual void on_key_pressed(key pressed_key) {};

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

    void draw(drawing_type type, const vertex_array& array,
              const shaders::shader_program& shaders);

    template <typename value_type>
    void draw(drawing_type type, const vertex_vector_array<value_type>& array,
              const shaders::shader_program& shaders) {

        draw(type, array.get_vertex_array(), shaders);
    }
}
