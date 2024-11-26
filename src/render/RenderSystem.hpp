#pragma once

#include <GL/glew.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/text/TextRenderer.hpp"
#include "shader/CyberpunkBackgroundShader.hpp"

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

   private:
    void render_background(glm::uvec2 drawable_size);
    void render_text(glm::uvec2 drawable_size, entt::registry &registry);

    struct CyberpunkBackground {
        CyberpunkBackgroundShader shader;
        std::vector<glm::vec2> vertices;
        GLuint vao;
        GLuint vbo;

        CyberpunkBackground();
        ~CyberpunkBackground();

        CyberpunkBackground(const CyberpunkBackground &other) = delete;
        CyberpunkBackground &operator=(const CyberpunkBackground &other) =
            delete;

        CyberpunkBackground(CyberpunkBackground &&other) = delete;
        CyberpunkBackground &operator=(CyberpunkBackground &&other) = delete;
    };

    CyberpunkBackground background_;
    TextRenderer text_renderer;
};