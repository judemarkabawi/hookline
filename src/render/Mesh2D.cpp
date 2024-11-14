#include "Mesh2D.hpp"

Mesh2D Mesh2D::from_verts(std::vector<Vertex> verts) {
    Mesh2D result;
    result.verts = std::move(verts);
    result.setup();
    return result;
}

Mesh2D::~Mesh2D() { cleanup(); }

Mesh2D::Mesh2D(Mesh2D &&other)
    : vao(other.vao), vbo(other.vbo), verts(std::move(other.verts)) {
    other.vao = 0;
    other.vbo = 0;
}

Mesh2D &Mesh2D::operator=(Mesh2D &&other) {
    if (this == &other) {
        return *this;
    }

    // Clean up current objects
    cleanup();

    // Move
    vao = other.vao;
    vbo = other.vbo;
    verts = std::move(other.verts);

    // Reset other
    other.vao = 0;
    other.vbo = 0;

    return *this;
}

void Mesh2D::setup() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind VAO
    glBindVertexArray(vao);

    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(),
                 GL_STATIC_DRAW);

    // -- Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // -- Texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(1);
    // -- Color
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);
}

void Mesh2D::cleanup() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}