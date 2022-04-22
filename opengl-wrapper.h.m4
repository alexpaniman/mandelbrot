#pragma once

#include "opengl-error-handler.h"
#include <GL/glew.h>

namespace gl::raw {
include(stdlib.m4)dnl
divert(-1)

define(`SIGNATURES', `(
GLuint glCreateProgram(),
GLuint glCreateShader(GLenum shaderType),
void glAttachShader(GLuint program, GLuint shader),
void glBindBuffer(GLenum target, GLuint buffer),
void glBindVertexArray(GLuint array),
void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage),
void glClear(GLbitfield mask),
void glCompileShader(GLuint shader),
void glDeleteBuffers(GLsizei n, const GLuint * buffers),
void glDeleteProgram(GLuint program),
void glDrawArrays(GLenum mode, GLint first, GLsizei count),
void glEnableVertexAttribArray(GLuint index),
void glGenBuffers(GLsizei n, GLuint * buffers),
void glGenVertexArrays(GLsizei n, GLuint *arrays),
void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog),
void glGetShaderiv(GLuint shader, GLenum pname, GLint *params),
void glLinkProgram(GLuint program),
void glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length),
void glUseProgram(GLuint program),
void glValidateProgram(GLuint program),
void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer))')

divert(0)dnl

    #ifndef NDEBUG
    #define GL_CLEAR_ERROR() gl::error::clear_error()
    #define GL_CHECK_ERROR() gl::error::check_error()
    #else
    #define GL_CLEAR_ERROR() ((void) 0)
    #define GL_CHECK_ERROR() ((void) 0)
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
define(FUNCTION_CALL,
    `patsubst(patsubst(signature, `^.*gl', `gl'),
	      `\([(,] *\)[^(),]*[ *]\(\w+\)', `\1\2')')

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
define(RETURNS_VOID,
    `ifelse(patsubst(signature, `\(\w+\).*', `\1'), `void', `1', `0')')


divert(0)dnl
    inline patsubst(signature, `\(\w+ *\).*', `\1')`'dnl
patsubst(CAMEL_TO_SNAKE_CASE(patsubst(signature, `\w+ +\(\w+\)(.*)', `\1')), `gl_', `')`'dnl
patsubst(signature, `^.*(', `(') {
        GL_CLEAR_ERROR();
        ifelse(RETURNS_VOID, `1', `', `auto result = ')dnl
FUNCTION_CALL;
        GL_CHECK_ERROR();ifelse(RETURNS_VOID, `1', `', `

        return result;')
    }
')
    #undef GL_CLEAR_ERROR
    #undef GL_CHECK_ERROR

};
