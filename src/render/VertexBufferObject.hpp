#pragma once

#include <GL/glew.h>

struct VertexBufferObject {
    GLuint vbo = 0;

    VertexBufferObject();
    ~VertexBufferObject();

    VertexBufferObject(const VertexBufferObject& other) = delete;
    VertexBufferObject& operator=(const VertexBufferObject& other) = delete;

    VertexBufferObject(VertexBufferObject&& other) noexcept;
    VertexBufferObject& operator=(VertexBufferObject&& other) noexcept;
};