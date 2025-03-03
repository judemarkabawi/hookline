#include "PlayMode.hpp"

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "constants.hpp"
#include "core/Game.hpp"
#include "core/InputComponent.hpp"
#include "core/Level.hpp"
#include "core/TransformComponent.hpp"
#include "gameplay/HealthComponent.hpp"
#include "gameplay/ProjectileSystem.hpp"
#include "physics/ColliderComponent.hpp"
#include "physics/GrapplingHook.hpp"
#include "physics/PhysicsSystem.hpp"
#include "physics/RigidBodyComponent.hpp"
#include "render/CameraComponent.hpp"
#include "render/CameraSystem.hpp"
#include "render/RenderComponent.hpp"
#include "util/misc.hpp"

PlayMode::PlayMode()
    : collectables(&asset_manager), projectileSystem(&asset_manager) {
    // Need to load level before camera
    setup_map();

    setup_camera();

    // Play music
    Sound::loop(asset_manager.get_sound("guitar_loop_music"), 0.25);
}

void PlayMode::update(float dt, Game &game) {
    /* -- PLAYER INPUT & GRAPPLE -- */
    auto &player_transform = registry.get<TransformComponent>(player_.entity);
    auto [grapple_transform, grapple_renderable, grapple] =
        registry
            .get<TransformComponent, RenderComponent, GrapplingHookComponent>(
                grapple_entity);

    // TODO: Put input into a separate input component and handle this movement
    auto &inputs = registry.get<InputComponent>(player_.entity);
    inputs.movement = {0.0f, 0.0f};
    if (player_.up.pressed) {
        if (grapple.attached) {
            float force = hookline::player_movement_force.y;
            glm::vec2 forceVec =
                force * grapple.get_up_dir(player_transform.position);
            inputs.movement += forceVec;
        }
    }
    if (player_.down.pressed) {
        if (grapple.attached) {
            float force = hookline::player_movement_force.y;
            glm::vec2 forceVec =
                force * grapple.get_down_dir(player_transform.position);
            inputs.movement += forceVec;
        }
    }
    if (player_.left.pressed) {
        if (grapple.attached) {
            float force = 2.0f * hookline::player_movement_force.x;
            glm::vec2 forceVec =
                force * grapple.get_left_dir(player_transform.position);
            inputs.movement += forceVec;
        } else {
            float force = hookline::player_movement_force.x;
            glm::vec2 forceVec = force * glm::vec2{-1.0f, 0.0f};
            inputs.movement += forceVec;
        }
    }
    if (player_.right.pressed) {
        if (grapple.attached) {
            float force = 2.0f * hookline::player_movement_force.x;
            glm::vec2 forceVec =
                force * grapple.get_right_dir(player_transform.position);
            inputs.movement += forceVec;
        } else {
            float force = hookline::player_movement_force.x;
            glm::vec2 forceVec = force * glm::vec2{1.0f, 0.0f};
            inputs.movement += forceVec;
        }
    }

    //   Track grapple to player
    grapple_transform.position = player_transform.position;

    //   Convert mouse click position to world position
    auto [camera_transform, camera] =
        registry.get<TransformComponent, CameraComponent>(camera_entity);
    glm::vec2 target_world_position = hookline::convert_opengl_mouse_to_world(
        player_.mouse.position, camera_transform.position, camera.viewport_size,
        camera.pixels_per_unit);

    //   Attach or detach grapple
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
    auto &health = registry.get<HealthComponent>(player_.entity);
    if (health.health <= 0) {
        if (grapple.attached) {
            grapple.detach();
        }
        registry.remove<RigidBodyComponent>(player_.entity);
        registry.remove<RenderComponent>(player_.entity);
        if (registry.all_of<ColliderComponent>(player_.entity)) {
            registry.remove<ColliderComponent>(player_.entity);
        }
        game.change_mode(GameMode::Mode::GameOverMenuMode);
    }

    if (player_transform.position[1] < -5.f) {
        game.change_mode(GameMode::Mode::WinMenuMode);
    }

    // System updates
    physics.update(dt, registry);
    camera_system->update(dt, registry);
    collectables.update(dt, registry, player_.entity);
    projectileSystem.update(dt, registry, player_.entity);
    collisions.update(dt, registry);
}

void PlayMode::render(glm::uvec2 drawable_size, Game &_) {
    // Render scene
    rendering.render(drawable_size, registry, camera_entity);
}

bool PlayMode::handle_event(SDL_Event const &event, glm::uvec2 drawable_size,
                            Game &game) {
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
        } else if (event.key.keysym.sym == SDLK_ESCAPE) {
            game.change_mode(GameMode::Mode::PauseMenuMode);
            return true;
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            player_.mouse.pressed = true;
            player_.mouse.position =
                hookline::screen_to_opengl({x, y}, drawable_size);
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            player_.mouse.pressed = false;
        }
    }
    return false;
}

void PlayMode::setup_camera() {
    camera_system = std::make_unique<CameraSystem>(player_.entity);
    auto camera = registry.create();
    registry.emplace<TransformComponent>(camera, glm::vec2{0.0f, 0.0f},
                                         glm::vec2{1.0f, 1.0f}, 0.0f);
    registry.emplace<CameraComponent>(camera, glm::vec2{0.0f, 0.0f},
                                      hookline::pixels_per_unit);
    camera_entity = camera;
}

void PlayMode::setup_map() {
    auto level = Level::load_json(
        hookline::data_path("../../assets/levels/basic_level.json"));
    asset_manager = std::move(level.assets);
    registry = std::move(level.registry);
    player_.entity = std::move(level.player);
    grapple_entity = std::move(level.grapple);

    // load bacgrounds shader
    rendering.load_background_images(&asset_manager);
}
