#pragma once

#include "opengl-error-handler.h"
#include <GL/glew.h>

namespace gl::raw {

    #ifndef NDEBUG
    #define GL_CLEAR_ERROR() gl::error::clear_error()
    #define GL_CHECK_ERROR() gl::error::check_error()
    #else
    #define GL_CLEAR_ERROR() ((void) 0)
    #define GL_CHECK_ERROR() ((void) 0)
    #endif

    inline GLuint create_program() {
        GL_CLEAR_ERROR();
        auto result = glCreateProgram();
        GL_CHECK_ERROR();

        return result;
    }

    inline GLuint create_shader(GLenum shaderType) {
        GL_CLEAR_ERROR();
        auto result = glCreateShader(shaderType);
        GL_CHECK_ERROR();

        return result;
    }

    inline void attach_shader(GLuint program, GLuint shader) {
        GL_CLEAR_ERROR();
        glAttachShader(program, shader);
        GL_CHECK_ERROR();
    }

    inline void bind_buffer(GLenum target, GLuint buffer) {
        GL_CLEAR_ERROR();
        glBindBuffer(target, buffer);
        GL_CHECK_ERROR();
    }

    inline void bind_vertex_array(GLuint array) {
        GL_CLEAR_ERROR();
        glBindVertexArray(array);
        GL_CHECK_ERROR();
    }

    inline void buffer_data(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage) {
        GL_CLEAR_ERROR();
        glBufferData(target, size, data, usage);
        GL_CHECK_ERROR();
    }

    inline void clear(GLbitfield mask) {
        GL_CLEAR_ERROR();
        glClear(mask);
        GL_CHECK_ERROR();
    }

    inline void compile_shader(GLuint shader) {
        GL_CLEAR_ERROR();
        glCompileShader(shader);
        GL_CHECK_ERROR();
    }

    inline void delete_buffers(GLsizei n, const GLuint * buffers) {
        GL_CLEAR_ERROR();
        glDeleteBuffers(n, buffers);
        GL_CHECK_ERROR();
    }

    inline void delete_program(GLuint program) {
        GL_CLEAR_ERROR();
        glDeleteProgram(program);
        GL_CHECK_ERROR();
    }

    inline void draw_arrays(GLenum mode, GLint first, GLsizei count) {
        GL_CLEAR_ERROR();
        glDrawArrays(mode, first, count);
        GL_CHECK_ERROR();
    }

    inline void enable_vertex_attrib_array(GLuint index) {
        GL_CLEAR_ERROR();
        glEnableVertexAttribArray(index);
        GL_CHECK_ERROR();
    }

    inline void gen_buffers(GLsizei n, GLuint * buffers) {
        GL_CLEAR_ERROR();
        glGenBuffers(n, buffers);
        GL_CHECK_ERROR();
    }

    inline void gen_vertex_arrays(GLsizei n, GLuint *arrays) {
        GL_CLEAR_ERROR();
        glGenVertexArrays(n, arrays);
        GL_CHECK_ERROR();
    }

    inline void get_shader_info_log(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog) {
        GL_CLEAR_ERROR();
        glGetShaderInfoLog(shader, maxLength, length, infoLog);
        GL_CHECK_ERROR();
    }

    inline void get_shaderiv(GLuint shader, GLenum pname, GLint *params) {
        GL_CLEAR_ERROR();
        glGetShaderiv(shader, pname, params);
        GL_CHECK_ERROR();
    }

    inline void link_program(GLuint program) {
        GL_CLEAR_ERROR();
        glLinkProgram(program);
        GL_CHECK_ERROR();
    }

    inline void shader_source(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
        GL_CLEAR_ERROR();
        glShaderSource(shader, count, string, length);
        GL_CHECK_ERROR();
    }

    inline void use_program(GLuint program) {
        GL_CLEAR_ERROR();
        glUseProgram(program);
        GL_CHECK_ERROR();
    }

    inline void validate_program(GLuint program) {
        GL_CLEAR_ERROR();
        glValidateProgram(program);
        GL_CHECK_ERROR();
    }

    inline void vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) {
        GL_CLEAR_ERROR();
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        GL_CHECK_ERROR();
    }

    #undef GL_CLEAR_ERROR
    #undef GL_CHECK_ERROR

};
