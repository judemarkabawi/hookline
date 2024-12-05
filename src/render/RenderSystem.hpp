#pragma once

#include <GL/glew.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/AssetManager.hpp"
#include "core/text/TextRenderer.hpp"
#include "render/Mesh2D.hpp"
#include "shader/BasicMeshShader.hpp"
#include "shader/CollectibleShader.hpp"
#include "shader/CyberpunkBackgroundShader.hpp"
#include "shader/CyberpunkBackgroundShaderFull.hpp"
#include "shader/GrappleBoxShader.hpp"
#include "shader/PlayerShader.hpp"
#include "shader/ProjectileShader.hpp"
#include "shader/WallShader.hpp"
#include "util/misc.hpp"

/**
   The RenderSystem is very inefficient right now. It rebinds VAOs and VBOs for
   every single renderable every frame. Should fix at some point by just
   defining VBOs for every static mesh which a RenderComponent would then
   borrow/have a reference to. Then the render per frame would just update some
   uniforms and rebind VAO.
 */
class RenderSystem {
   public:
    RenderSystem();

    void render(glm::uvec2 drawable_size, entt::registry &registry,
                entt::entity camera);
    void render_text(glm::uvec2 drawable_size, entt::registry &registry);

    void load_background_images(AssetManager *assets);

    void render_menu_background(glm::uvec2 drawable_size);

   private:
    void render_background(glm::uvec2 drawable_size, glm::vec2 camera_pos);

   private:
    void bind_textures();
    void unbind_textures();

    template <typename Shader>
    struct Background {
        Mesh2D mesh = Mesh2D::from_verts_color(
            hookline::get_basic_shape_debug(), {0, 0, 0, 0});
        Shader shader;
    };

    TextRenderer text_renderer;

    BasicMeshShader mesh_shader;
    GrappleBoxShader grapple_shader;
    CollectibleShader collectible_shader;
    ProjectileShader projectile_shader;
    PlayerShader player_shader;
    WallShader wall_shader;

    Background<CyberpunkBackgroundShaderFull> background_;
    Background<CyberpunkBackgroundShader> menu_background_;
};
