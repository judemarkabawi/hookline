#pragma once

#include <GL/glew.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

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
    void render(glm::uvec2 drawable_size, entt::registry &registry,
                entt::entity camera);

   private:
    void render_background(glm::uvec2 drawable_size);

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

        CyberpunkBackground(const CyberpunkBackground &&other) = delete;
        CyberpunkBackground &operator=(const CyberpunkBackground &&other) =
            delete;
    };
    CyberpunkBackground background_;
};