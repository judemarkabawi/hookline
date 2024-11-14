#pragma once

#include <GL/glew.h>

#include <vector>

#include "render/Vertex.hpp"

struct Mesh2D {
    GLuint vao = 0;  // owning
    GLuint vbo = 0;  // owning
    std::vector<Vertex> verts;

    /* Constructors */
    static Mesh2D from_verts(std::vector<Vertex> vertices);

    Mesh2D() = default;
    ~Mesh2D();

    Mesh2D(const Mesh2D &other) = delete;
    Mesh2D &operator=(const Mesh2D &other) = delete;

    Mesh2D(Mesh2D &&other);
    Mesh2D &operator=(Mesh2D &&other);

   private:
    void setup();
    void cleanup();
};