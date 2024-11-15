#include "RenderSystem.hpp"

#include <chrono>
#include <cstddef>
#include <entt/entt.hpp>

#include "core/TransformComponent.hpp"
#include "render/CameraComponent.hpp"
#include "render/RenderComponent.hpp"
#include "shader/CyberpunkBackgroundShader.hpp"
#include "util/misc.hpp"

void RenderSystem::render(glm::uvec2 drawable_size, entt::registry &registry,
                          entt::entity camera_entity) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    render_background(drawable_size);

    // Update camera before rendering
    auto [camera_transform, camera] =
        registry.get<TransformComponent, CameraComponent>(camera_entity);
    camera.viewport_size = drawable_size;

    auto view = registry.view<TransformComponent, RenderComponent>();
    // Render each renderable
    for (const auto [entity, transform, renderable] : view.each()) {
        if (!renderable.visible_) {
            continue;
        }

        GLuint vao_ = renderable.vao_;
        GLuint vbo_ = renderable.vbo_;
        const auto &verts_ = renderable.verts_;
        const auto &program_ = renderable.program_;

        // Vertex data
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, verts_.size() * sizeof(Vertex),
                     verts_.data(), GL_STATIC_DRAW);

        // Vertex attribute data
        glBindVertexArray(vao_);
        // -- Position
        glVertexAttribPointer(program_.a_position_loc, 2, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex),
                              (void *)offsetof(Vertex, position));
        glEnableVertexAttribArray(program_.a_position_loc);
        // -- Texture coordinate
        glVertexAttribPointer(program_.a_texture_coord_loc, 2, GL_FLOAT,
                              GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, tex_coords));
        glEnableVertexAttribArray(program_.a_texture_coord_loc);
        // -- Color
        glVertexAttribPointer(program_.a_color_loc, 4, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), (void *)offsetof(Vertex, color));
        glEnableVertexAttribArray(program_.a_color_loc);

        // Use program
        glUseProgram(program_.program);

        // Set uniforms
        // -- Vertex shader
        glUniform2f(program_.u_position_loc, transform.position.x,
                    transform.position.y);
        glUniform2f(program_.u_scale_loc, transform.scale.x, transform.scale.y);
        glUniform1f(program_.u_rotation_loc, transform.rotation);
        glUniform2f(program_.u_camera_position_loc, camera_transform.position.x,
                    camera_transform.position.y);
        glUniform2f(program_.u_camera_viewport_size_loc, camera.viewport_size.x,
                    camera.viewport_size.y);
        glUniform1f(program_.u_camera_pixels_per_unit_loc,
                    camera.pixels_per_unit);
        // -- Fragment shader
        glUniform1i(program_.u_frag_use_texture_loc, renderable.use_texture_);

        // Texture
        if (renderable.use_texture_) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, renderable.texture_);
            glUniform1i(program_.u_frag_texture_loc, 0);
        }

        // Draw
        glDrawArrays(GL_TRIANGLE_STRIP, 0, verts_.size());

        // Cleanup
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

RenderSystem::CyberpunkBackground::CyberpunkBackground() {
    vertices = hookline::get_basic_shape_debug();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind VAO
    glBindVertexArray(vao);

    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2),
                 vertices.data(), GL_STATIC_DRAW);

    // Vertex attribute data
    glVertexAttribPointer(shader.m.a_position_loc, 2, GL_FLOAT, GL_FALSE,
                          sizeof(glm::vec2), (void *)0);
    glEnableVertexAttribArray(shader.m.a_position_loc);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

RenderSystem::CyberpunkBackground::~CyberpunkBackground() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void RenderSystem::render_background(glm::uvec2 drawable_size) {
    // Bind VAO
    glBindVertexArray(background_.vao);

    // Use program
    glUseProgram(background_.shader.m.program);

    // Uniforms
    // -- Fragment shader
    using namespace std::chrono;
    static auto start_time = high_resolution_clock::now();
    auto time_diff =
        duration_cast<milliseconds>(high_resolution_clock::now() - start_time);
    float time = time_diff.count();
    glUniform1f(background_.shader.m.u_time_loc, time);

    glUniform2f(background_.shader.m.u_drawable_size_loc,
                (float)drawable_size.x, (float)drawable_size.y);

    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, background_.vertices.size());
}