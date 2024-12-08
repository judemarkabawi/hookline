#include "Mesh2D.hpp"

Mesh2D Mesh2D::from_verts(std::vector<Vertex> verts) {
    Mesh2D result;
    result.verts = std::move(verts);
    result.setup();
    return result;
}

/**
 * Make a new RenderComponent using vertices and a (default) color.
 */
Mesh2D Mesh2D::from_verts_color(const std::vector<glm::vec2> &vertices,
                                glm::vec4 color) {
    std::vector<Vertex> verts;
    for (glm::vec2 vertex_pos : vertices) {
        Vertex vertex{vertex_pos, glm::vec2{0.0f, 0.0f}, color};
        verts.push_back(vertex);
    }
    return Mesh2D::from_verts(std::move(verts));
}

/**
 * Make a new RenderComponent using vertices and a texture;
 */
Mesh2D Mesh2D::from_verts_texture(const std::vector<glm::vec2> &vertices,
                                  const std::vector<glm::vec2> &tex_coords,
                                  glm::vec4 color) {
    assert(vertices.size() == tex_coords.size() &&
           "RenderComponent: Cannot construct with different length vertices "
           "and tex_coords");

    std::vector<Vertex> verts;
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vertex vertex{vertices[i], tex_coords[i], color};
        verts.push_back(vertex);
    }
    return Mesh2D::from_verts(std::move(verts));
}

GLuint Mesh2D::vao() const { return vao_.vao; }
GLuint Mesh2D::vbo() const { return vbo_.vbo; }

void Mesh2D::setup() {
    glGenVertexArrays(1, &vao_.vao);
    glGenBuffers(1, &vbo_.vbo);

    // Bind VAO
    glBindVertexArray(vao());

    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo());
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
