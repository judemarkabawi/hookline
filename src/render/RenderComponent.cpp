#include "RenderComponent.hpp"

#include "render/Mesh2D.hpp"
#include "render/Vertex.hpp"

/**
 * Make a new RenderComponent using vertices and a (default) color. Default type is BASE
 */
RenderComponent RenderComponent::from_vertices_color(
    const std::vector<glm::vec2>& vertices, glm::vec4 color,
            RenderType type) {
    Mesh2D mesh = Mesh2D::from_verts_color(vertices, color);

    RenderComponent result;
    result.type = type;
    result.mesh_ = std::move(mesh);
    result.visible_ = true;
    result.use_texture_ = false;
    return result;
}

/**
 * Make a new RenderComponent using vertices, (default) color, and tex_cords (but no texture). Default type is BASE
 */
RenderComponent RenderComponent::from_vertices_color_tex(
    const std::vector<glm::vec2>& vertices, glm::vec4 color,
            const std::vector<glm::vec2>& tex_coords,
            RenderType type) {
    assert(vertices.size() == tex_coords.size() &&
           "RenderComponent: Cannot construct with different length vertices "
           "and tex_coords");
        
    Mesh2D mesh = Mesh2D::from_verts_texture(vertices, tex_coords, color);

    RenderComponent result;
    result.type = type;
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
    const std::vector<glm::vec2>& tex_coords, GLuint texture,
    RenderType type) {
    assert(vertices.size() == tex_coords.size() &&
           "RenderComponent: Cannot construct with different length vertices "
           "and tex_coords");
    assert(texture != 0 && "RenderComponent: Need a valid texture");

    Mesh2D mesh = Mesh2D::from_verts_texture(vertices, tex_coords);

    RenderComponent result;
    result.type = type;
    result.texture_ = texture;
    result.mesh_ = std::move(mesh);
    result.visible_ = true;
    result.use_texture_ = true;
    return result;
}

void RenderComponent::set_visible(bool visible) { visible_ = visible; }
