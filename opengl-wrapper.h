#pragma once

#include "opengl-error-handler.h"

#include <iostream>
#include <GL/glew.h>

namespace gl::raw {

    #ifndef NDEBUG
    #define GL_CLEAR_ERROR() gl::error::clear_error()
    #define GL_CHECK_ERROR() gl::error::check_error()
    #else
    #define GL_CLEAR_ERROR() ((void) 0)
    #define GL_CHECK_ERROR() ((void) 0)
    #endif

    #ifdef GL_LOG_CALLS
    #define GL_LOG_CALL(name, args)       \
            std::cout << " ==> " << name; \
            std::cout << args << "\n";
    #else
    #define GL_LOG_CALL(name, args) ((void) 0)
    #endif

    inline GLuint create_program() {
        GL_LOG_CALL("glCreateProgram", "()");

        GL_CLEAR_ERROR();
        auto result = glCreateProgram();
        GL_CHECK_ERROR();        

        return result;
    }

    inline GLuint create_shader(GLenum shaderType) {
        GL_LOG_CALL("glCreateShader", "("
               " shaderType = " << shaderType
            << " )");

        GL_CLEAR_ERROR();
        auto result = glCreateShader(shaderType);
        GL_CHECK_ERROR();        

        return result;
    }

    inline void attach_shader(GLuint program, GLuint shader) {
        GL_LOG_CALL("glAttachShader", "("
               " program = " << program
            << " shader = " << shader
            << " )");

        GL_CLEAR_ERROR();
        glAttachShader(program, shader);
        GL_CHECK_ERROR();        
    }

    inline void bind_buffer(GLenum target, GLuint buffer) {
        GL_LOG_CALL("glBindBuffer", "("
               " target = " << target
            << " buffer = " << buffer
            << " )");

        GL_CLEAR_ERROR();
        glBindBuffer(target, buffer);
        GL_CHECK_ERROR();        
    }

    inline void bind_vertex_array(GLuint array) {
        GL_LOG_CALL("glBindVertexArray", "("
               " array = " << array
            << " )");

        GL_CLEAR_ERROR();
        glBindVertexArray(array);
        GL_CHECK_ERROR();        
    }

    inline void buffer_data(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage) {
        GL_LOG_CALL("glBufferData", "("
               " target = " << target
            << " size = " << size
            << " data = " << data
            << " usage = " << usage
            << " )");

        GL_CLEAR_ERROR();
        glBufferData(target, size, data, usage);
        GL_CHECK_ERROR();        
    }

    inline void clear(GLbitfield mask) {
        GL_LOG_CALL("glClear", "("
               " mask = " << mask
            << " )");

        GL_CLEAR_ERROR();
        glClear(mask);
        GL_CHECK_ERROR();        
    }

    inline void compile_shader(GLuint shader) {
        GL_LOG_CALL("glCompileShader", "("
               " shader = " << shader
            << " )");

        GL_CLEAR_ERROR();
        glCompileShader(shader);
        GL_CHECK_ERROR();        
    }

    inline void delete_buffers(GLsizei n, const GLuint * buffers) {
        GL_LOG_CALL("glDeleteBuffers", "("
               " n = " << n
            << " buffers = " << buffers
            << " )");

        GL_CLEAR_ERROR();
        glDeleteBuffers(n, buffers);
        GL_CHECK_ERROR();        
    }

    inline void delete_program(GLuint program) {
        GL_LOG_CALL("glDeleteProgram", "("
               " program = " << program
            << " )");

        GL_CLEAR_ERROR();
        glDeleteProgram(program);
        GL_CHECK_ERROR();        
    }

    inline void draw_arrays(GLenum mode, GLint first, GLsizei count) {
        GL_LOG_CALL("glDrawArrays", "("
               " mode = " << mode
            << " first = " << first
            << " count = " << count
            << " )");

        GL_CLEAR_ERROR();
        glDrawArrays(mode, first, count);
        GL_CHECK_ERROR();        
    }

    inline void enable_vertex_attrib_array(GLuint index) {
        GL_LOG_CALL("glEnableVertexAttribArray", "("
               " index = " << index
            << " )");

        GL_CLEAR_ERROR();
        glEnableVertexAttribArray(index);
        GL_CHECK_ERROR();        
    }

    inline void gen_buffers(GLsizei n, GLuint * buffers) {
        GL_LOG_CALL("glGenBuffers", "("
               " n = " << n
            << " buffers = " << buffers
            << " )");

        GL_CLEAR_ERROR();
        glGenBuffers(n, buffers);
        GL_CHECK_ERROR();        
    }

    inline void gen_vertex_arrays(GLsizei n, GLuint *arrays) {
        GL_LOG_CALL("glGenVertexArrays", "("
               " n = " << n
            << " arrays = " << arrays
            << " )");

        GL_CLEAR_ERROR();
        glGenVertexArrays(n, arrays);
        GL_CHECK_ERROR();        
    }

    inline void get_shader_info_log(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog) {
        GL_LOG_CALL("glGetShaderInfoLog", "("
               " shader = " << shader
            << " maxLength = " << maxLength
            << " length = " << length
            << " infoLog = " << infoLog
            << " )");

        GL_CLEAR_ERROR();
        glGetShaderInfoLog(shader, maxLength, length, infoLog);
        GL_CHECK_ERROR();        
    }

    inline void get_shaderiv(GLuint shader, GLenum pname, GLint *params) {
        GL_LOG_CALL("glGetShaderiv", "("
               " shader = " << shader
            << " pname = " << pname
            << " params = " << params
            << " )");

        GL_CLEAR_ERROR();
        glGetShaderiv(shader, pname, params);
        GL_CHECK_ERROR();        
    }

    inline void link_program(GLuint program) {
        GL_LOG_CALL("glLinkProgram", "("
               " program = " << program
            << " )");

        GL_CLEAR_ERROR();
        glLinkProgram(program);
        GL_CHECK_ERROR();        
    }

    inline void shader_source(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
        GL_LOG_CALL("glShaderSource", "("
               " shader = " << shader
            << " count = " << count
            << " string = " << string
            << " length = " << length
            << " )");

        GL_CLEAR_ERROR();
        glShaderSource(shader, count, string, length);
        GL_CHECK_ERROR();        
    }

    inline void use_program(GLuint program) {
        GL_LOG_CALL("glUseProgram", "("
               " program = " << program
            << " )");

        GL_CLEAR_ERROR();
        glUseProgram(program);
        GL_CHECK_ERROR();        
    }

    inline void validate_program(GLuint program) {
        GL_LOG_CALL("glValidateProgram", "("
               " program = " << program
            << " )");

        GL_CLEAR_ERROR();
        glValidateProgram(program);
        GL_CHECK_ERROR();        
    }

    inline void vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) {
        GL_LOG_CALL("glVertexAttribPointer", "("
               " index = " << index
            << " size = " << size
            << " type = " << type
            << " normalized = " << normalized
            << " stride = " << stride
            << " pointer = " << pointer
            << " )");

        GL_CLEAR_ERROR();
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        GL_CHECK_ERROR();        
    }

    #undef GL_CLEAR_ERROR
    #undef GL_CHECK_ERROR

};
