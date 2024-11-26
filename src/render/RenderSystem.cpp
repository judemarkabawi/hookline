#include "RenderSystem.hpp"

#include <chrono>
#include <cstdlib>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/TransformComponent.hpp"
#include "core/text/TextComponent.hpp"
#include "render/CameraComponent.hpp"
#include "render/Mesh2D.hpp"
#include "render/RenderComponent.hpp"
#include "shader/CyberpunkBackgroundShader.hpp"
#include "util/misc.hpp"

RenderSystem::RenderSystem() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);
}

void RenderSystem::render(glm::uvec2 drawable_size, entt::registry &registry,
                          entt::entity camera_entity) {
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

        const auto &verts_ = renderable.mesh_.verts;
        const auto &program_ = renderable.program_;

        // Vertex attribute data
        glBindVertexArray(renderable.mesh_.vao);

        // Use program
        glUseProgram(program_.m.program);

        // Uniforms
        // -- Vertex shader
        glUniform2f(program_.m.u_position_loc, transform.position.x,
                    transform.position.y);
        glUniform2f(program_.m.u_scale_loc, transform.scale.x,
                    transform.scale.y);
        glUniform1f(program_.m.u_rotation_loc, transform.rotation);
        glUniform2f(program_.m.u_camera_position_loc,
                    camera_transform.position.x, camera_transform.position.y);
        glUniform2f(program_.m.u_camera_viewport_size_loc,
                    camera.viewport_size.x, camera.viewport_size.y);
        glUniform1f(program_.m.u_camera_pixels_per_unit_loc,
                    camera.pixels_per_unit);
        // -- Fragment shader
        glUniform1i(program_.m.u_frag_use_texture_loc, renderable.use_texture_);

        // Texture
        if (renderable.use_texture_) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, renderable.texture_);
            glUniform1i(program_.m.u_frag_texture_loc, 0);
        }

        // Draw
        glDrawArrays(GL_TRIANGLE_STRIP, 0, verts_.size());
    }

    render_text(drawable_size, registry);
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

/**
 * Render all text objects
 *
 * TODO: Inefficent - should only shape text once/on text update, not per frame
 *
 * TODO: Inefficient - move scaling by drawable_size to an ortho matrix, not
 * manual
 */
void RenderSystem::render_text(glm::uvec2 drawable_size,
                               entt::registry &registry) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Render every text component
    auto view = registry.view<TextComponent>();
    for (const auto [_, text] : view.each()) {
        // Vertex attribute data
        glBindVertexArray(text.mesh.vao);

        // Text shader program
        glUseProgram(text.shader.m.program);

        glm::vec2 scale_to_screen =
            2.0f / glm::vec2{drawable_size.x, drawable_size.y};

        glm::vec2 scaled_position = text.position / scale_to_screen;
        float x = scaled_position.x;  // Starting x position
        float y = scaled_position.y;  // Starting y position
        float scale = text.scale;     // Scale for the text rendering

        // Shape text and generate glyph map -- inefficent
        GlyphData glyphs_data = text_renderer.shape_text(text.text);

        // Iterate over all glyphs in the glyph map and render them:
        std::vector<Glyph> glyphs =
            text_renderer.generate_glyph_textures(glyphs_data);
        for (auto const &glyph_entry : glyphs) {
            Glyph ch = glyph_entry;

            // Calculate the position and size of the current glyph:
            float xpos = x + ch.bearing.x * scale;
            float ypos = y - (ch.size.y - ch.bearing.y) * scale;
            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            // Update the content of VBO memory with the quad vertices:
            text.mesh.verts[0].position =
                glm::vec2{xpos, ypos + h} * scale_to_screen;
            text.mesh.verts[1].position =
                glm::vec2{xpos + w, ypos + h} * scale_to_screen;
            text.mesh.verts[2].position =
                glm::vec2{xpos, ypos} * scale_to_screen;
            text.mesh.verts[3].position =
                glm::vec2{xpos + w, ypos} * scale_to_screen;
            glBindBuffer(GL_ARRAY_BUFFER, text.mesh.vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            text.mesh.verts.size() * sizeof(Vertex),
                            text.mesh.verts.data());

            // Render glyph texture over quad:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ch.texture);
            glUniform1i(text.shader.m.u_frag_texture_loc, 0);

            // Render the quad:
            glDrawArrays(GL_TRIANGLE_STRIP, 0, text.mesh.verts.size());

            // Advance the cursor for the next glyph (accounting for scaling):
            // Bitshift by 6 to convert from 1/64th pixels to pixels
            x += (ch.advance) * scale;
        }
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}