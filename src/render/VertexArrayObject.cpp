#include "VertexArrayObject.hpp"

VertexArrayObject::VertexArrayObject() { glGenVertexArrays(1, &this->vao); }

VertexArrayObject::~VertexArrayObject() { glDeleteVertexArrays(1, &this->vao); }

VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) noexcept
    : vao(other.vao) {
    other.vao = 0;
}
VertexArrayObject& VertexArrayObject::operator=(
    VertexArrayObject&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    glDeleteVertexArrays(1, &this->vao);

    this->vao = other.vao;
    other.vao = 0;

    return *this;
}