#pragma once

#include <GL/glew.h>

struct ShaderProgram {
    GLuint program = 0;

    ShaderProgram() = default;
    ShaderProgram(GLuint program);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram& other) = delete;
    ShaderProgram& operator=(const ShaderProgram& other) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;
};