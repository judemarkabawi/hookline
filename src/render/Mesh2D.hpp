#pragma once

#include <GL/glew.h>

#include <vector>

#include "render/Vertex.hpp"
#include "render/VertexArrayObject.hpp"
#include "render/VertexBufferObject.hpp"

struct Mesh2D {
    std::vector<Vertex> verts;

    /* Constructors */
    Mesh2D() = default;

    static Mesh2D from_verts(std::vector<Vertex> vertices);
    static Mesh2D from_verts_color(const std::vector<glm::vec2> &vertices,
                                   glm::vec4 color);
    static Mesh2D from_verts_texture(const std::vector<glm::vec2> &vertices,
                                     const std::vector<glm::vec2> &tex_coords,
                                     glm::vec4 color = glm::vec4(0.0, 0.0, 0.0,
                                                                 1.0));
    GLuint vao() const;
    GLuint vbo() const;

   private:
    VertexArrayObject vao_;
    VertexBufferObject vbo_;

    void setup();
};