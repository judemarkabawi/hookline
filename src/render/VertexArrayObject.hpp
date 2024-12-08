#pragma once

#include <GL/glew.h>

struct VertexArrayObject {
    GLuint vao = 0;

    VertexArrayObject();
    ~VertexArrayObject();

    VertexArrayObject(const VertexArrayObject& other) = delete;
    VertexArrayObject& operator=(const VertexArrayObject& other) = delete;

    VertexArrayObject(VertexArrayObject&& other) noexcept;
    VertexArrayObject& operator=(VertexArrayObject&& other) noexcept;
};