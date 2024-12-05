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

    void render_background(glm::uvec2 drawable_size, glm::vec2 camera_pos);

    template <typename Shader>
    struct Background {
        Mesh2D mesh = Mesh2D::from_verts_color(
            hookline::get_basic_shape_debug(), {0, 0, 0, 0});
        Shader shader;
    };

   private:
    void bind_textures();
    void unbind_textures();

    TextRenderer text_renderer;

    static inline bool loaded = false;
    static inline std::unique_ptr<BasicMeshShader> mesh_shader = nullptr;
    static inline std::unique_ptr<GrappleBoxShader> grapple_shader = nullptr;
    static inline std::unique_ptr<CollectibleShader> collectible_shader = nullptr;
    static inline std::unique_ptr<ProjectileShader> projectile_shader = nullptr;
    static inline std::unique_ptr<PlayerShader> player_shader = nullptr;

    static inline std::unique_ptr<Background<CyberpunkBackgroundShaderFull>> background_ = nullptr;
    static inline std::unique_ptr<Background<CyberpunkBackgroundShader>> menu_background_ = nullptr;
};
