#pragma once

#include <GL/glew.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/text/TextRenderer.hpp"
#include "shader/CyberpunkBackgroundShaderFull.hpp"
#include "core/AssetManager.hpp"

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
    void load_background_images(AssetManager *assets);

   private:
    void render_background(glm::uvec2 drawable_size, glm::vec2 camera_pos);
    void render_text(glm::uvec2 drawable_size, entt::registry &registry);

    struct CyberpunkBackground {
        CyberpunkBackgroundShaderFull shader;
        std::vector<glm::vec2> vertices;
        GLuint vao;
        GLuint vbo;

        //textures
        GLuint bg_emission = -1U;
        GLuint bg_color = -1U;
        GLuint bg_normals = -1U;
        GLuint mg_emission = -1U;
        GLuint mg_color = -1U;
        GLuint mg_normals = -1U;
        GLuint fg_emission = -1U;
        GLuint fg_color = -1U;
        GLuint fg_normals = -1U;
        GLuint bg_cube = -1U;

        CyberpunkBackground();
        ~CyberpunkBackground();

        CyberpunkBackground(const CyberpunkBackground &other) = delete;
        CyberpunkBackground &operator=(const CyberpunkBackground &other) =
            delete;

        CyberpunkBackground(CyberpunkBackground &&other) = delete;
        CyberpunkBackground &operator=(CyberpunkBackground &&other) = delete;
    };

    void bind_textures();
    void unbind_textures();


    CyberpunkBackground background_;
    TextRenderer text_renderer;
};