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

    inline GLint get_uniform_location(GLuint program, const GLchar *name) {
        GL_LOG_CALL("glGetUniformLocation", "("
               " program = " << program
            << " name = " << name
            << " )");

        GL_CLEAR_ERROR();
        auto result = glGetUniformLocation(program, name);
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

    inline void begin(GLenum mode) {
        GL_LOG_CALL("glBegin", "("
               " mode = " << mode
            << " )");

        GL_CLEAR_ERROR();
        glBegin(mode);
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

    inline void buffer_data(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) {
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

    inline void color3f(GLfloat red, GLfloat green, GLfloat blue) {
        GL_LOG_CALL("glColor3f", "("
               " red = " << red
            << " green = " << green
            << " blue = " << blue
            << " )");

        GL_CLEAR_ERROR();
        glColor3f(red, green, blue);
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

    inline void delete_buffers(GLsizei n, const GLuint *buffers) {
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

    inline void end() {
        GL_LOG_CALL("glEnd", "()");

        GL_CLEAR_ERROR();
        glEnd();
        GL_CHECK_ERROR();        
    }

    inline void gen_buffers(GLsizei n, GLuint *buffers) {
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

    inline void uniform1f(GLint location, GLfloat v0) {
        GL_LOG_CALL("glUniform1f", "("
               " location = " << location
            << " v0 = " << v0
            << " )");

        GL_CLEAR_ERROR();
        glUniform1f(location, v0);
        GL_CHECK_ERROR();        
    }

    inline void uniform1fv(GLint location, GLsizei count, const GLfloat *value) {
        GL_LOG_CALL("glUniform1fv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform1fv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform1d(GLint location, GLdouble v0) {
        GL_LOG_CALL("glUniform1d", "("
               " location = " << location
            << " v0 = " << v0
            << " )");

        GL_CLEAR_ERROR();
        glUniform1d(location, v0);
        GL_CHECK_ERROR();        
    }

    inline void uniform1dv(GLint location, GLsizei count, const GLdouble *value) {
        GL_LOG_CALL("glUniform1dv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform1dv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform1i(GLint location, GLint v0) {
        GL_LOG_CALL("glUniform1i", "("
               " location = " << location
            << " v0 = " << v0
            << " )");

        GL_CLEAR_ERROR();
        glUniform1i(location, v0);
        GL_CHECK_ERROR();        
    }

    inline void uniform1iv(GLint location, GLsizei count, const GLint *value) {
        GL_LOG_CALL("glUniform1iv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform1iv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform1ui(GLint location, GLuint v0) {
        GL_LOG_CALL("glUniform1ui", "("
               " location = " << location
            << " v0 = " << v0
            << " )");

        GL_CLEAR_ERROR();
        glUniform1ui(location, v0);
        GL_CHECK_ERROR();        
    }

    inline void uniform1uiv(GLint location, GLsizei count, const GLuint *value) {
        GL_LOG_CALL("glUniform1uiv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform1uiv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform2f(GLint location, GLfloat v0, GLfloat v1) {
        GL_LOG_CALL("glUniform2f", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " )");

        GL_CLEAR_ERROR();
        glUniform2f(location, v0, v1);
        GL_CHECK_ERROR();        
    }

    inline void uniform2fv(GLint location, GLsizei count, const GLfloat *value) {
        GL_LOG_CALL("glUniform2fv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform2fv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform2d(GLint location, GLdouble v0, GLdouble v1) {
        GL_LOG_CALL("glUniform2d", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " )");

        GL_CLEAR_ERROR();
        glUniform2d(location, v0, v1);
        GL_CHECK_ERROR();        
    }

    inline void uniform2dv(GLint location, GLsizei count, const GLdouble *value) {
        GL_LOG_CALL("glUniform2dv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform2dv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform2i(GLint location, GLint v0, GLint v1) {
        GL_LOG_CALL("glUniform2i", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " )");

        GL_CLEAR_ERROR();
        glUniform2i(location, v0, v1);
        GL_CHECK_ERROR();        
    }

    inline void uniform2iv(GLint location, GLsizei count, const GLint *value) {
        GL_LOG_CALL("glUniform2iv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform2iv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform2ui(GLint location, GLuint v0, GLuint v1) {
        GL_LOG_CALL("glUniform2ui", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " )");

        GL_CLEAR_ERROR();
        glUniform2ui(location, v0, v1);
        GL_CHECK_ERROR();        
    }

    inline void uniform2uiv(GLint location, GLsizei count, const GLuint *value) {
        GL_LOG_CALL("glUniform2uiv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform2uiv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
        GL_LOG_CALL("glUniform3f", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " v2 = " << v2
            << " )");

        GL_CLEAR_ERROR();
        glUniform3f(location, v0, v1, v2);
        GL_CHECK_ERROR();        
    }

    inline void uniform3fv(GLint location, GLsizei count, const GLfloat *value) {
        GL_LOG_CALL("glUniform3fv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform3fv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform3d(GLint location, GLdouble v0, GLdouble v1, GLdouble v2) {
        GL_LOG_CALL("glUniform3d", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " v2 = " << v2
            << " )");

        GL_CLEAR_ERROR();
        glUniform3d(location, v0, v1, v2);
        GL_CHECK_ERROR();        
    }

    inline void uniform3dv(GLint location, GLsizei count, const GLdouble *value) {
        GL_LOG_CALL("glUniform3dv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform3dv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform3i(GLint location, GLint v0, GLint v1, GLint v2) {
        GL_LOG_CALL("glUniform3i", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " v2 = " << v2
            << " )");

        GL_CLEAR_ERROR();
        glUniform3i(location, v0, v1, v2);
        GL_CHECK_ERROR();        
    }

    inline void uniform3iv(GLint location, GLsizei count, const GLint *value) {
        GL_LOG_CALL("glUniform3iv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform3iv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2) {
        GL_LOG_CALL("glUniform3ui", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " v2 = " << v2
            << " )");

        GL_CLEAR_ERROR();
        glUniform3ui(location, v0, v1, v2);
        GL_CHECK_ERROR();        
    }

    inline void uniform3uiv(GLint location, GLsizei count, const GLuint *value) {
        GL_LOG_CALL("glUniform3uiv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform3uiv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
        GL_LOG_CALL("glUniform4f", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " v2 = " << v2
            << " v3 = " << v3
            << " )");

        GL_CLEAR_ERROR();
        glUniform4f(location, v0, v1, v2, v3);
        GL_CHECK_ERROR();        
    }

    inline void uniform4fv(GLint location, GLsizei count, const GLfloat *value) {
        GL_LOG_CALL("glUniform4fv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform4fv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform4d(GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) {
        GL_LOG_CALL("glUniform4d", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " v2 = " << v2
            << " v3 = " << v3
            << " )");

        GL_CLEAR_ERROR();
        glUniform4d(location, v0, v1, v2, v3);
        GL_CHECK_ERROR();        
    }

    inline void uniform4dv(GLint location, GLsizei count, const GLdouble *value) {
        GL_LOG_CALL("glUniform4dv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform4dv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
        GL_LOG_CALL("glUniform4i", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " v2 = " << v2
            << " v3 = " << v3
            << " )");

        GL_CLEAR_ERROR();
        glUniform4i(location, v0, v1, v2, v3);
        GL_CHECK_ERROR();        
    }

    inline void uniform4iv(GLint location, GLsizei count, const GLint *value) {
        GL_LOG_CALL("glUniform4iv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform4iv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
        GL_LOG_CALL("glUniform4ui", "("
               " location = " << location
            << " v0 = " << v0
            << " v1 = " << v1
            << " v2 = " << v2
            << " v3 = " << v3
            << " )");

        GL_CLEAR_ERROR();
        glUniform4ui(location, v0, v1, v2, v3);
        GL_CHECK_ERROR();        
    }

    inline void uniform4uiv(GLint location, GLsizei count, const GLuint *value) {
        GL_LOG_CALL("glUniform4uiv", "("
               " location = " << location
            << " count = " << count
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniform4uiv(location, count, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform_matrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
        GL_LOG_CALL("glUniformMatrix2fv", "("
               " location = " << location
            << " count = " << count
            << " transpose = " << transpose
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniformMatrix2fv(location, count, transpose, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform_matrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
        GL_LOG_CALL("glUniformMatrix2x3fv", "("
               " location = " << location
            << " count = " << count
            << " transpose = " << transpose
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniformMatrix2x3fv(location, count, transpose, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform_matrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
        GL_LOG_CALL("glUniformMatrix2x4fv", "("
               " location = " << location
            << " count = " << count
            << " transpose = " << transpose
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniformMatrix2x4fv(location, count, transpose, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform_matrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
        GL_LOG_CALL("glUniformMatrix3fv", "("
               " location = " << location
            << " count = " << count
            << " transpose = " << transpose
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniformMatrix3fv(location, count, transpose, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform_matrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
        GL_LOG_CALL("glUniformMatrix3x2fv", "("
               " location = " << location
            << " count = " << count
            << " transpose = " << transpose
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniformMatrix3x2fv(location, count, transpose, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform_matrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
        GL_LOG_CALL("glUniformMatrix3x4fv", "("
               " location = " << location
            << " count = " << count
            << " transpose = " << transpose
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniformMatrix3x4fv(location, count, transpose, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform_matrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
        GL_LOG_CALL("glUniformMatrix4fv", "("
               " location = " << location
            << " count = " << count
            << " transpose = " << transpose
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniformMatrix4fv(location, count, transpose, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform_matrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
        GL_LOG_CALL("glUniformMatrix4x2fv", "("
               " location = " << location
            << " count = " << count
            << " transpose = " << transpose
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniformMatrix4x2fv(location, count, transpose, value);
        GL_CHECK_ERROR();        
    }

    inline void uniform_matrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
        GL_LOG_CALL("glUniformMatrix4x3fv", "("
               " location = " << location
            << " count = " << count
            << " transpose = " << transpose
            << " value = " << value
            << " )");

        GL_CLEAR_ERROR();
        glUniformMatrix4x3fv(location, count, transpose, value);
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

    inline void vertex2f(GLfloat x, GLfloat y) {
        GL_LOG_CALL("glVertex2f", "("
               " x = " << x
            << " y = " << y
            << " )");

        GL_CLEAR_ERROR();
        glVertex2f(x, y);
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
