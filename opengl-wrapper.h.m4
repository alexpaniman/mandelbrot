#pragma once

#include "opengl-error-handler.h"

#include <iostream>
#include <GL/glew.h>

namespace gl::raw {
include(stdlib.m4)dnl
divert(-1)

define(`SIGNATURES', `(
GLuint glCreateProgram(),
GLint glGetUniformLocation(GLuint program, const GLchar *name),
GLuint glCreateShader(GLenum shaderType),
void glAttachShader(GLuint program, GLuint shader),
void glBegin(GLenum mode),
void glBindBuffer(GLenum target, GLuint buffer),
void glBindVertexArray(GLuint array),
void glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage),
void glClear(GLbitfield mask),
void glColor3f(GLfloat red, GLfloat green, GLfloat blue),
void glCompileShader(GLuint shader),
void glDeleteBuffers(GLsizei n, const GLuint *buffers),
void glDeleteProgram(GLuint program),
void glDrawArrays(GLenum mode, GLint first, GLsizei count),
void glEnableVertexAttribArray(GLuint index),
void glEnd(),
void glGenBuffers(GLsizei n, GLuint *buffers),
void glGenVertexArrays(GLsizei n, GLuint *arrays),
void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog),
void glGetShaderiv(GLuint shader, GLenum pname, GLint *params),
void glLinkProgram(GLuint program),
void glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length),
void glUniform1f(GLint location, GLfloat v0),
void glUniform1fv(GLint location, GLsizei count, const GLfloat *value),
void glUniform1d(GLint location, GLdouble v0),
void glUniform1dv(GLint location, GLsizei count, const GLdouble *value),
void glUniform1i(GLint location, GLint v0),
void glUniform1iv(GLint location, GLsizei count, const GLint *value),
void glUniform1ui(GLint location, GLuint v0),
void glUniform1uiv(GLint location, GLsizei count, const GLuint *value),
void glUniform2f(GLint location, GLfloat v0, GLfloat v1),
void glUniform2fv(GLint location, GLsizei count, const GLfloat *value),
void glUniform2d(GLint location, GLdouble v0, GLdouble v1),
void glUniform2dv(GLint location, GLsizei count, const GLdouble *value),
void glUniform2i(GLint location, GLint v0, GLint v1),
void glUniform2iv(GLint location, GLsizei count, const GLint *value),
void glUniform2ui(GLint location, GLuint v0, GLuint v1),
void glUniform2uiv(GLint location, GLsizei count, const GLuint *value),
void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2),
void glUniform3fv(GLint location, GLsizei count, const GLfloat *value),
void glUniform3d(GLint location, GLdouble v0, GLdouble v1, GLdouble v2),
void glUniform3dv(GLint location, GLsizei count, const GLdouble *value),
void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2),
void glUniform3iv(GLint location, GLsizei count, const GLint *value),
void glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2),
void glUniform3uiv(GLint location, GLsizei count, const GLuint *value),
void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3),
void glUniform4fv(GLint location, GLsizei count, const GLfloat *value),
void glUniform4d(GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3),
void glUniform4dv(GLint location, GLsizei count, const GLdouble *value),
void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3),
void glUniform4iv(GLint location, GLsizei count, const GLint *value),
void glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3),
void glUniform4uiv(GLint location, GLsizei count, const GLuint *value),
void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),
void glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),
void glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),
void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),
void glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),
void glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),
void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),
void glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),
void glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),
void glUseProgram(GLuint program),
void glValidateProgram(GLuint program),
void glVertex2f(GLfloat x, GLfloat y),
void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer))')

divert(0)dnl

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
foreach(signature, SIGNATURES,
`
divert(-1)

# ---------------------------------------------------------------
# FUNCTION_CALL = function signature with return type, and all
# arguments types stripped away.
#
# This implementation implies OpenGL naming style, look and
# RETURNS_VOID description to get an idea about possible limitations.
# ---------------------------------------------------------------
define(`FUNCTION_CALL',
    `patsubst(patsubst(signature, `^.*gl', `gl'),
              `\([(,] *\)[^(),]*[ *]\(\w+\)', `\1\2')')

define(`RETURN_TYPE', `patsubst(signature, `\(\w+ *\).*', `\1')')

# ---------------------------------------------------------------
# RETURNS_VOID = 0 if function returns something, 1 otherwise

# Compares function return type, that is, in this approximation,
# is denoted by the first word in function declaration.

# It works for all OpenGL functions since they are in C and do not
# have any kind of templates (with whitespace and <, >)

# And it would also completly fail if return type is denoted by
# inline struct or even just uses struct at the begining

# Also, because of OpenGL using camel case for typenames, it
# does not even consider underscores. Beware!
# ---------------------------------------------------------------
define(`RETURNS_VOID',
    `ifelse(patsubst(signature, `\(\w+\).*', `\1'), `void', `1', `0')')

define(`FUNCTION_NAME', `patsubst(signature, `\w+ +\(\w+\)(.*)', `\1')') 
define(`FUNCTION_NAME_SNAKE_CASED', `CAMEL_TO_SNAKE_CASE(FUNCTION_NAME)')

# ---------------------------------------------------------------
# FUNCTION_NAME = snake cased original name with "gl" prefix
# stripped away from the name.
# ---------------------------------------------------------------
define(`NEW_FUNCTION_NAME', `patsubst(FUNCTION_NAME_SNAKE_CASED, `gl_', `')')

define(`COMMA_SEPARATED_ARGS_IN_PARENS', `patsubst(FUNCTION_CALL, `.*?\((.*)\).*?', `\1')')

define(`NO_ARGS', `ifelse(patsubst(FUNCTION_CALL, `.*().*', `NO ARGS'), `NO ARGS', `1', `0')')

# ---------------------------------------------------------------
# NAMED_ARGS = function args separated by << and with << "name = "
# added before each one of them.
#
# Intended for use with std::cout-like string streams.
# ---------------------------------------------------------------
define(`NAMED_ARGS', "("
               `patsubst(COMMA_SEPARATED_ARGS_IN_PARENS,
                       `(?\(\w+?\)\([,)]\)', `" \1 = " << \1
            <<')' " )")

define(`NO_RETURN_TYPE_SIGNATURE', `patsubst(signature, `^.*(', `(')');

divert(0)dnl
    inline RETURN_TYPE`'NEW_FUNCTION_NAME`'NO_RETURN_TYPE_SIGNATURE {
        ifelse(NO_ARGS, `1',
        `GL_LOG_CALL("FUNCTION_NAME", "()");',
        `GL_LOG_CALL("FUNCTION_NAME", NAMED_ARGS);')

        GL_CLEAR_ERROR();
        ifelse(RETURNS_VOID, `1', `', `auto result = ')FUNCTION_CALL;
        GL_CHECK_ERROR();dnl
        ifelse(RETURNS_VOID, `1', `', `

        return result;')
    }
')
    #undef GL_CLEAR_ERROR
    #undef GL_CHECK_ERROR

};
