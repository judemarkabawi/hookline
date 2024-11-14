#include "RenderComponent.hpp"

#include "render/Mesh2D.hpp"
#include "render/Vertex.hpp"

/**
 * Make a new RenderComponent using vertices and a (default) color.
 */
RenderComponent RenderComponent::from_vertices_color(
    const std::vector<glm::vec2>& vertices, glm::vec4 color) {
    std::vector<Vertex> verts;
    for (glm::vec2 vertex_pos : vertices) {
        Vertex vertex{vertex_pos, glm::vec2{0.0f, 0.0f}, color};
        verts.push_back(vertex);
    }
    Mesh2D mesh = Mesh2D::from_verts(std::move(verts));

    RenderComponent result;
    result.mesh_ = std::move(mesh);
    result.visible_ = true;
    result.use_texture_ = false;
    return result;
}

/**
 * Make a new RenderComponent using vertices and a texture;
 */
RenderComponent RenderComponent::from_vertices_texture(
    const std::vector<glm::vec2>& vertices,
    const std::vector<glm::vec2>& tex_coords, GLuint texture) {
    assert(vertices.size() == tex_coords.size() &&
           "RenderComponent: Cannot construct with different length vertices "
           "and tex_coords");
    assert(texture != 0 && "RenderComponent: Need a valid texture");

    std::vector<Vertex> verts(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vertex vertex{vertices[i], tex_coords[i], {0.0f, 0.0f, 0.0f, 1.0f}};
        verts.push_back(vertex);
    }
    Mesh2D mesh = Mesh2D::from_verts(std::move(verts));

    RenderComponent result;
    result.texture_ = texture;
    result.mesh_ = std::move(mesh);
    result.visible_ = true;
    result.use_texture_ = true;
    return result;
}

void RenderComponent::set_visible(bool visible) { visible_ = visible; }
