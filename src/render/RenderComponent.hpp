#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <vector>

#include "render/Mesh2D.hpp"

/**
 * RenderComponent holding rendering data like a shader progrma, vertices, etc.
 *
 * Don't construct directly, call the `static RenderComponent from_***(...)`
 * methods to get an instance.
 */
class RenderComponent {
   public:
    /**
     * Make a new RenderComponent using vertices and a (default) color.
     */
    enum RenderType {
        BASE,
        GRAPPLE_POINT,
        COLLECTIBLE,
        PROJECTILE,
        PLAYER,
        WALL,
        ROPE
    };

    RenderType type = BASE;

    static RenderComponent from_vertices_color(
        const std::vector<glm::vec2>& vertices,
        glm::vec4 color = {0.0f, 0.0f, 0.0f, 1.0f}, RenderType type = BASE);
    /**
     * Make a new RenderComponent using vertices and a texture;
     */
    static RenderComponent from_vertices_texture(
        const std::vector<glm::vec2>& vertices,
        const std::vector<glm::vec2>& tex_coords, GLuint texture,
        RenderType type = BASE);

    static RenderComponent from_vertices_color_tex(
        const std::vector<glm::vec2>& vertices,
        glm::vec4 color = {0.0f, 0.0f, 0.0f, 1.0f},
        const std::vector<glm::vec2>& tex_coords = {{0.0f, 0.0f},
                                                    {1.0, 0.0},
                                                    {0.0, 1.0},
                                                    {1.0, 1.0}},
        RenderType type = BASE);

    ~RenderComponent() = default;

    // Cannot copy a render component
    RenderComponent(const RenderComponent& other) = delete;
    RenderComponent& operator=(const RenderComponent& other) = delete;

    RenderComponent(RenderComponent&& other) noexcept = default;
    RenderComponent& operator=(RenderComponent&& other) noexcept = default;

    void set_visible(bool visible);

    Mesh2D mesh_;
    GLuint texture_ = 0;  // non-owning
    bool visible_ = true;
    bool use_texture_ = false;

   private:
    RenderComponent() = default;
};