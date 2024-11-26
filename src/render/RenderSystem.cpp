#include "RenderSystem.hpp"

#include <chrono>
#include <cstdlib>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/TransformComponent.hpp"
#include "core/text/TextComponent.hpp"
#include "core/AssetManager.hpp"
#include "render/CameraComponent.hpp"
#include "render/Mesh2D.hpp"
#include "render/RenderComponent.hpp"
#include "gameplay/ProjectileComponent.hpp"
#include "gameplay/HealthComponent.hpp"
#include "shader/CyberpunkBackgroundShaderFull.hpp"
#include "util/misc.hpp"
#include "constants.hpp"

RenderSystem::RenderSystem() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);
}


void RenderSystem::render(glm::uvec2 drawable_size, entt::registry &registry,
                          entt::entity camera_entity) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Update camera before rendering
    auto [camera_transform, camera] =
        registry.get<TransformComponent, CameraComponent>(camera_entity);
    camera.viewport_size = drawable_size;

    render_background(drawable_size, camera_transform.position);

    auto view = registry.view<TransformComponent, RenderComponent>();
    // Render each renderable
    for (const auto [entity, transform, renderable] : view.each()) {
        if (!renderable.visible_) {
            continue;
        }

        const auto &verts_ = renderable.mesh_.verts;

        //const auto &program_ = renderable.program_;

        // Vertex attribute data
        glBindVertexArray(renderable.mesh_.vao);

        using namespace std::chrono;
        static auto start_time = high_resolution_clock::now();
        auto time_diff = duration_cast<milliseconds>(high_resolution_clock::now() - start_time);
        float time = time_diff.count();
        float u_time = time/1000.0f; //in seconds
        if(renderable.type == RenderComponent::RenderType::BASE) {
            glUseProgram(mesh_shader.m.program);
            mesh_shader.updateUniforms(transform.position, transform.scale, transform.rotation,
                                       camera_transform.position, camera.viewport_size, camera.pixels_per_unit,
                                       renderable.use_texture_, renderable.texture_);
        } else if (renderable.type == RenderComponent::RenderType::GRAPPLE_POINT) {
            glUseProgram(grapple_shader.m.program);
            grapple_shader.updateUniforms(transform.position, transform.scale, transform.rotation,
                                       camera_transform.position, camera.viewport_size, camera.pixels_per_unit,
                                       renderable.use_texture_, renderable.texture_, u_time);
        } else if (renderable.type == RenderComponent::RenderType::COLLECTIBLE) {
            glUseProgram(collectible_shader.m.program);
            collectible_shader.updateUniforms(transform.position, transform.scale, transform.rotation,
                                          camera_transform.position, camera.viewport_size, camera.pixels_per_unit,
                                          drawable_size, u_time, hookline::collectible_glow_ratio);
            glBlendFunc(GL_ONE, GL_ONE);
        } else if (renderable.type == RenderComponent::RenderType::PROJECTILE) {
            glUseProgram(projectile_shader.m.program);
            auto projectile = registry.get<ProjectileComponent>(entity);
            projectile_shader.updateUniforms(transform.position, transform.scale, 0.0,
                                            camera_transform.position, camera.viewport_size, camera.pixels_per_unit,
                                            u_time, projectile.direction, hookline::projectile_glow_ratio, projectile.currtime/projectile.lifetime);
            glBlendFunc(GL_ONE, GL_ONE);
        } else if (renderable.type == RenderComponent::RenderType::PLAYER) {
            glUseProgram(player_shader.m.program);
            auto health = registry.get<HealthComponent>(entity);
            //printf("%f\n", (float)health.health/(float)health.inital_health);
            player_shader.updateUniforms(transform.position, transform.scale, transform.rotation,
                                            camera_transform.position, camera.viewport_size, camera.pixels_per_unit,
                                            u_time, (float)health.health/(float)health.inital_health);
        }

        // Draw
        glDrawArrays(GL_TRIANGLE_STRIP, 0, verts_.size());
        if(renderable.type == RenderComponent::RenderType::COLLECTIBLE || renderable.type == RenderComponent::RenderType::PROJECTILE) {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
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

void RenderSystem::load_background_images(AssetManager* manager) {
        //load associated textures
    background_.bg_emission = manager->load_texture("bg_emission", hookline::data_path("../../assets/textures/bg_emission.png"), GL_TEXTURE_2D, GL_NEAREST, GL_CLAMP);
    background_.bg_color = manager->load_texture("bg_color", hookline::data_path("../../assets/textures/bg_color.png"), GL_TEXTURE_2D, GL_LINEAR, GL_REPEAT);
    background_.bg_normals = manager->load_texture("bg_normals", hookline::data_path("../../assets/textures/bg_normals.png"), GL_TEXTURE_2D, GL_NEAREST, GL_CLAMP);

    background_.mg_emission = manager->load_texture("bg_emission", hookline::data_path("../../assets/textures/bg_emission.png"), GL_TEXTURE_2D, GL_NEAREST, GL_CLAMP);
    background_.mg_color = manager->load_texture("bg_color", hookline::data_path("../../assets/textures/bg_color.png"), GL_TEXTURE_2D, GL_LINEAR, GL_REPEAT);
    background_.mg_normals = manager->load_texture("bg_normals", hookline::data_path("../../assets/textures/bg_normals.png"), GL_TEXTURE_2D, GL_NEAREST, GL_CLAMP);

    background_.fg_emission = manager->load_texture("fg_emission", hookline::data_path("../../assets/textures/fg_emission.png"), GL_TEXTURE_2D, GL_NEAREST, GL_CLAMP);
    background_.fg_color = manager->load_texture("fg_color", hookline::data_path("../../assets/textures/fg_color.png"), GL_TEXTURE_2D, GL_LINEAR, GL_REPEAT);
    background_.fg_normals = manager->load_texture("fg_normals", hookline::data_path("../../assets/textures/fg_normals.png"), GL_TEXTURE_2D, GL_NEAREST, GL_CLAMP);

    background_.bg_cube = manager->load_texture("bg_cube_", hookline::data_path("../../assets/textures/bg_cube_"), GL_TEXTURE_CUBE_MAP, GL_LINEAR, GL_REPEAT);

}

void RenderSystem::bind_textures() {
    //assume program is already bound
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, background_.bg_emission);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, background_.bg_color);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, background_.bg_normals);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, background_.mg_emission);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, background_.mg_color);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, background_.mg_normals);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, background_.fg_emission);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, background_.fg_color);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, background_.fg_normals);

    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_CUBE_MAP, background_.bg_cube);
}

void RenderSystem::unbind_textures() {
    //assume program is already bound
    for(int i = 0; i < 9; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glActiveTexture(GL_TEXTURE0 + 9);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glActiveTexture(GL_TEXTURE0);
}

RenderSystem::CyberpunkBackground::~CyberpunkBackground() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void RenderSystem::render_background(glm::uvec2 drawable_size, glm::vec2 camera_pos) {
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
    glUniform1f(background_.shader.m.u_time_loc, time/1000.0);

    glUniform2f(background_.shader.m.u_drawable_size_loc,
                (float)drawable_size.x, (float)drawable_size.y);
    glUniform2f(background_.shader.m.u_camera_pos,
                (float)camera_pos.x, (float)camera_pos.y);

    bind_textures();

    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, background_.vertices.size());
    glUseProgram(0); //unbind
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

RenderSystem::MenuBackground::MenuBackground() {
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

RenderSystem::MenuBackground::~MenuBackground() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void RenderSystem::render_menu_background(glm::uvec2 drawable_size) {
    // Bind VAO
    glBindVertexArray(menu_background_.vao);

    // Use program
    glUseProgram(menu_background_.shader.m.program);

    // Uniforms
    // -- Fragment shader
    using namespace std::chrono;
    static auto start_time = high_resolution_clock::now();
    auto time_diff =
        duration_cast<milliseconds>(high_resolution_clock::now() - start_time);
    float time = time_diff.count();
    glUniform1f(menu_background_.shader.m.u_time_loc, time);

    glUniform2f(menu_background_.shader.m.u_drawable_size_loc,
                (float)drawable_size.x, (float)drawable_size.y);

    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, menu_background_.vertices.size());
}