/**
 * @file Game.cpp
 *
 * Implement Game.hpp
 */

#include "Game.hpp"

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "constants.hpp"
#include "core/InputComponent.hpp"
#include "core/TransformComponent.hpp"
#include "physics/Components.hpp"
#include "physics/GrapplingHook.hpp"
#include "physics/PhysicsSystem.hpp"
#include "physics/util.hpp"
#include "render/CameraComponent.hpp"
#include "render/CameraSystem.hpp"
#include "render/RenderComponent.hpp"
#include "util/misc.hpp"

namespace {
/**
    Helper to make a manually defined box on the map.
 */
entt::entity hookable_box(entt::registry &registry, glm::vec2 position,
                          glm::vec2 scale) {
    auto box = registry.create();
    registry.emplace<TransformComponent>(
        box, TransformComponent(position, scale, 0.0f));
    registry.emplace<RigidBodyComponent>(box);
    registry.emplace<ColliderComponent>(
        box, ColliderComponent(glm::vec2{1.0f, 1.0f}, true, false, true));
    registry.emplace<RenderComponent>(
        box, RenderComponent::from_vertices_color(
                 hookline::get_basic_shape_debug(), {0.07, 0.11, 0.23, 1.0}));
    return box;
}

/**
    Helper to make a manually defined maybe-hookable box on the map.
 */
entt::entity maybe_hookable_box(entt::registry &registry, glm::vec2 position,
                                glm::vec2 scale, bool hookable = false) {
    auto box = registry.create();
    registry.emplace<TransformComponent>(
        box, TransformComponent(position, scale, 0.0f));
    registry.emplace<RigidBodyComponent>(box);
    registry.emplace<ColliderComponent>(
        box, ColliderComponent(glm::vec2{1.0f, 1.0f}, true, false, hookable));
    registry.emplace<RenderComponent>(box,
                                      RenderComponent::from_vertices_color(
                                          hookline::get_basic_shape_debug()));
    return box;
}
}  // namespace

Game::Game() : collectables(&asset_manager) {
    // Load assets
    // TODO: A game/level should read its required assets from some file and
    // load those as appropriate.
    load_assets();

    // Player and attached camera setup
    setup_player();
    setup_camera();

    // All hookable platforms, ground, and collectables
    setup_map();

    // Play music
    Sound::loop(asset_manager.get_sound("guitar_loop_music"), 0.25);
}

void Game::update(float dt) {
    /* -- PLAYER INPUT & GRAPPLE -- */
    // TODO: Put input into a separate input component and handle this movement
    auto &inputs = registry.get<InputComponent>(player_.entity);
    inputs.movement = {0.0f, 0.0f};
    if (player_.up.pressed) {
        inputs.movement += glm::vec2{0.0f, hookline::player_movement_force.y};
    }
    if (player_.down.pressed) {
        inputs.movement += glm::vec2{0, -hookline::player_movement_force.y};
    }
    if (player_.left.pressed) {
        inputs.movement += glm::vec2{-hookline::player_movement_force.x, 0.0f};
    }
    if (player_.right.pressed) {
        inputs.movement += glm::vec2{hookline::player_movement_force.x, 0.0f};
    }
    // Grapple inputs - all manual stuff, fix later
    auto &player_transform = registry.get<TransformComponent>(player_.entity);
    auto [grapple_transform, grapple_renderable, grapple] =
        registry
            .get<TransformComponent, RenderComponent, GrapplingHookComponent>(
                grapple_entity);
    grapple_transform.position = player_transform.position;

    // Convert mouse click position to world position
    auto [camera_transform, camera] =
        registry.get<TransformComponent, CameraComponent>(camera_entity);
    glm::vec2 target_world_position = hookline::convert_opengl_mouse_to_world(
        player_.mouse.position, camera_transform.position, camera.viewport_size,
        camera.pixels_per_unit);

    if (player_.mouse.pressed) {
        grapple.try_attach(player_transform.position, target_world_position,
                           registry);
    }
    if (!player_.mouse.pressed) {
        grapple.detach();
    }
    // Grapple rendering setup
    grapple_renderable.set_visible(grapple.attached);
    if (grapple.attached) {
        grapple_transform.position =
            (player_transform.position + grapple.attached_position) / 2.0f;
        grapple_transform.scale = {glm::distance(player_transform.position,
                                                 grapple.attached_position) /
                                       2.0f,
                                   0.01};
        glm::vec2 direction = glm::normalize(grapple.attached_position -
                                             player_transform.position);
        grapple_transform.rotation = -glm::atan(direction.y, direction.x);
    }

    // System updates
    physics.update(dt, registry);
    collisions.update(dt, registry);
    camera_system->update(dt, registry);
    collectables.update(dt, registry, player_.entity);
}

void Game::render(glm::uvec2 drawable_size) {
    // Render scene
    rendering.render(drawable_size, registry, camera_entity);
}

bool Game::handle_event(SDL_Event const &event, glm::uvec2 drawable_size) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_a) {
            player_.left.pressed = true;
            return true;
        } else if (event.key.keysym.sym == SDLK_d) {
            player_.right.pressed = true;
            return true;
        } else if (event.key.keysym.sym == SDLK_w) {
            player_.up.pressed = true;
            return true;
        } else if (event.key.keysym.sym == SDLK_s) {
            player_.down.pressed = true;
            return true;
        }
    } else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_a) {
            player_.left.pressed = false;
            return true;
        } else if (event.key.keysym.sym == SDLK_d) {
            player_.right.pressed = false;
            return true;
        } else if (event.key.keysym.sym == SDLK_w) {
            player_.up.pressed = false;
            return true;
        } else if (event.key.keysym.sym == SDLK_s) {
            player_.down.pressed = false;
            return true;
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            player_.mouse.pressed = true;
            player_.mouse.position =
                hookline::convert_mouse_to_opengl(x, y, drawable_size);
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            player_.mouse.pressed = false;
        }
    }
    return false;
}

void Game::load_assets() {
    asset_manager.load_sound(
        "item_pick_up",
        hookline::data_path("../assets/sounds/item_pick_up.opus"));
    asset_manager.load_sound(
        "guitar_loop_music",
        hookline::data_path("../assets/sounds/guitar_loop_music.opus"));
}

void Game::setup_player() {
    auto player = registry.create();
    registry.emplace<TransformComponent>(
        player, TransformComponent(glm::vec2{0.0f, 0.0f},
                                   glm::vec2{0.05f, 0.05f}, 0.0f));
    registry.emplace<RigidBodyComponent>(player);
    registry.emplace<ForceComponent>(player);
    registry.emplace<ColliderComponent>(player, glm::vec2{1.0f, 1.0f}, true,
                                        true, false);
    registry.emplace<RenderComponent>(player,
                                      RenderComponent::from_vertices_color(
                                          hookline::get_basic_shape_debug()));
    registry.emplace<InputComponent>(player);
    player_.entity = player;
}

void Game::setup_camera() {
    camera_system = std::make_unique<CameraSystem>(player_.entity);
    auto camera = registry.create();
    registry.emplace<TransformComponent>(camera, glm::vec2{0.0f, 0.0f},
                                         glm::vec2{1.0f, 1.0f}, 0.0f);
    registry.emplace<CameraComponent>(camera, glm::vec2{0.0f, 0.0f},
                                      hookline::pixels_per_unit);
    camera_entity = camera;
}

void Game::setup_map() {
    glm::vec2 bottom_left = {-2.0f, -2.0f};
    glm::vec2 top_right = {2.0f, 3.0f};

    srand(time(nullptr));

    // Create a few immovable boxes somewhere
    {
        for (size_t i = 0; i < 10; ++i) {
            glm::vec2 position = glm::linearRand(bottom_left, top_right);
            hookable_box(registry, position, glm::vec2{0.05f, 0.05f});
        }
    }

    // Create a ground, side walls, and ceiling
    {
        maybe_hookable_box(registry, {0, -3.1f}, {3.0f, 0.05f});
        maybe_hookable_box(registry, {-3.0f, 0.0f}, {0.05f, 3.0f});
        maybe_hookable_box(registry, {3.0f, 0.0f}, {0.05f, 3.0f});
        maybe_hookable_box(registry, {0, 3.1f}, {3.0f, 0.05f});
    }

    // Create grappling hook
    {
        grapple_entity = registry.create();
        registry.emplace<TransformComponent>(
            grapple_entity, TransformComponent(glm::vec2(0.0f, 0.0f),
                                               glm::vec2{0.05f, 0.05f}, 0.0f));
        registry.emplace<GrapplingHookComponent>(grapple_entity, grapple_entity,
                                                 player_.entity);
        registry.emplace<RenderComponent>(
            grapple_entity, RenderComponent::from_vertices_color(
                                hookline::get_basic_shape_debug()));
    }

    // Spawn some collectables
    {
        for (size_t i = 0; i < 20; ++i) {
            collectables.spawn_random(registry);
        }
    }
}