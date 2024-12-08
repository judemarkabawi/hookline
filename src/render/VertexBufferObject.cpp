#include "VertexBufferObject.hpp"

VertexBufferObject::VertexBufferObject() { glGenBuffers(1, &this->vbo); }

VertexBufferObject::~VertexBufferObject() { glDeleteBuffers(1, &this->vbo); }

VertexBufferObject::VertexBufferObject(VertexBufferObject&& other) noexcept
    : vbo(other.vbo) {
    other.vbo = 0;
}
VertexBufferObject& VertexBufferObject::operator=(
    VertexBufferObject&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    glDeleteBuffers(1, &this->vbo);

    this->vbo = other.vbo;
    other.vbo = 0;

    return *this;
}