add_library(gl STATIC
  opengl-setup.cpp vertex-array.cpp vertex-buffer.cpp
  vertex-layout.cpp math.cpp opengl-error-handler.cpp uniforms.cpp)

target_include_directories(gl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# Setup m4 preprocessor for opengl-wrapper.h.m4

add_custom_command(OUTPUT opengl-wrapper.h
    COMMAND m4 -I ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/opengl-wrapper.h.m4 >
      ${CMAKE_CURRENT_SOURCE_DIR}/opengl-wrapper.h
    DEPENDS opengl-wrapper.h.m4
    COMMENT "Generating wrapper for OpenGL calls" )

add_custom_target(run-m4-opengl-wrapper-generator
    DEPENDS opengl-wrapper.h
    COMMENT "Checking if opengl-wrapper.h re-generation is required" )

add_dependencies(gl run-m4-opengl-wrapper-generator)


# Link OpenGL, GLEW and GLFW with gl

set(OpenGL_GL_PREFERENCE GLVND)

find_package(GLEW   REQUIRED)
find_package(glfw3  REQUIRED)
find_package(OpenGL REQUIRED)

target_include_directories(
    gl PUBLIC
    ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

target_link_libraries(gl PUBLIC ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw)
