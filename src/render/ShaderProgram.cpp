#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(GLuint program) : program(program) {}

ShaderProgram::~ShaderProgram() { glDeleteProgram(program); }

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : program(other.program) {
    other.program = 0;
}
ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    glDeleteProgram(this->program);

    this->program = other.program;
    other.program = 0;

    return *this;
}