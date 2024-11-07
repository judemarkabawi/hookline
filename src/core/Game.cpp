/**
 * @file Game.cpp
 *
 * Implement Game.hpp
 */

#include "Game.hpp"

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>

#include <iostream>

#include "constants.hpp"
#include "core/InputComponent.hpp"
#include "gameplay/HealthComponent.hpp"
#include "gameplay/ProjectileComponent.hpp"
#include "core/RenderComponent.hpp"
#include "core/TransformComponent.hpp"
#include "physics/Components.hpp"
#include "physics/GrapplingHook.hpp"
#include "physics/PhysicsSystem.hpp"
#include "physics/util.hpp"
#include "util/misc.hpp"

void Game::generate_fixed_tiles(const LevelData &level_data) {
        for (const auto &position : level_data.tile_positions) {
            auto box = registry.create();
            registry.emplace<TransformComponent>(
                box,
                TransformComponent(position, glm::vec2{0.05f, 0.05f}, 0.0f));
            registry.emplace<RigidBodyComponent>(box);
            registry.emplace<ColliderComponent>(
                box, ColliderComponent(glm::vec2{1.0f, 1.0f}, true, false));
            registry.emplace<RenderComponent>(
                box, hookline::get_basic_shape_debug(), true);
        }
}

Game::Game() {
    // Initialize player
    {
        auto player = registry.create();
        registry.emplace<TransformComponent>(
            player, TransformComponent(glm::vec2{0.0f, 0.0f},
                                       glm::vec2{0.05f, 0.05f}, 0.0f));
        registry.emplace<RigidBodyComponent>(player);
        registry.emplace<ForceComponent>(player);
        registry.emplace<ColliderComponent>(player, glm::vec2{1.0f, 1.0f});
        registry.emplace<RenderComponent>(player,
                                          hookline::get_basic_shape_debug());
        registry.emplace<InputComponent>(player);
        registry.emplace<HealthComponent>(player, 3);
        player_.entity = player;
    }

    {
        LevelData fixed_level = {
    {
        // Initial arrow pattern
        {0.2f, 0.1f}, {0.1f, 0.0f}, {0.4f, 0.1f}, {0.5f, 0.0f},
        {0.3f, -0.1f}, {0.3f, 0.0f}, {0.3f, 0.1f}, {0.3f, 0.2f},
        {0.3f, -0.2f}, {0.3f, -0.3f},
        
        // Section 1: Height 0.0 to 2.0
        {-0.6f, 0.5f}, {0.3f, 1.2f}, {0.8f, 1.8f}, {0.2f, 0.7f}, {-0.4f, 1.5f},
        
        // Section 2: Height 2.0 to 4.0
        {0.6f, 2.5f}, {-0.5f, 3.1f}, {0.1f, 2.8f}, {-0.3f, 3.7f},
        
        // Section 3: Height 4.0 to 6.0
        {0.7f, 4.2f}, {0.3f, 5.1f}, {-0.4f, 4.5f}, {0.5f, 5.8f}, {-0.2f, 4.8f},
        
        // Section 4: Height 6.0 to 8.0
        {-0.3f, 6.5f}, {0.2f, 7.3f}, {0.6f, 7.9f}, {-0.1f, 7.5f},
        
        // Section 5: Height 8.0 to 10.0
        {0.1f, 8.2f}, {-0.6f, 8.5f}, {0.3f, 9.4f}, {-0.2f, 9.0f}, {0.4f, 9.7f},
        
        // Section 6: Height 10.0 to 12.0
        {-0.5f, 10.3f}, {0.6f, 11.1f}, {0.2f, 10.6f}, {-0.4f, 11.8f}, {0.0f, 10.9f},
        
        // Section 7: Height 12.0 to 14.0
        {0.5f, 12.5f}, {-0.6f, 13.2f}, {0.3f, 13.7f}, {0.1f, 12.9f},
        
        // Section 8: Height 14.0 to 16.0
        {-0.3f, 14.5f}, {0.7f, 15.0f}, {0.2f, 15.4f}, {0.4f, 14.9f},
        
        // Section 9: Height 16.0 to 18.0
        {0.0f, 16.7f}, {-0.6f, 17.3f}, {0.5f, 17.9f}, {0.2f, 16.1f},
        
        // Section 10: Height 18.0 to 20.0
        {0.1f, 18.4f}, {-0.5f, 18.9f}, {0.4f, 19.3f}, {-0.2f, 19.8f}
    }
};


        generate_fixed_tiles(fixed_level);
    }
    /*
    // Create a few immovable boxes somewhere
    {
        std::vector<glm::vec2> positions = {glm::vec2{0.5f, 0.5f},
                                            glm::vec2{-0.5f, 0.5f},
                                            glm::vec2{-0.5f, -0.5f}};
        for (const auto &position : positions) {
            auto box = registry.create();
            registry.emplace<TransformComponent>(
                box,
                TransformComponent(position, glm::vec2{0.05f, 0.05f}, 0.0f));
            registry.emplace<RigidBodyComponent>(box);
            registry.emplace<ColliderComponent>(
                box, ColliderComponent(glm::vec2{1.0f, 1.0f}, true, false));
            registry.emplace<RenderComponent>(
                box, hookline::get_basic_shape_debug(), true);
        }
    }
    

    {
         // Randomly generate tiles
        std::srand(static_cast<unsigned int>(std::time(nullptr)));  // Seed for randomness
        int num_tiles = 40;  // Number of tiles to generate
        float max_height = 10.0f;  // Maximum vertical height of the level

        for (int i = 0; i < num_tiles; ++i) {
            auto tile = registry.create();
            
            float x_position = (std::rand() % 100 / 100.0f) * 2.0f - 1.0f;  // Random x within screen width
            float y_position = (std::rand() % 100 / 100.0f) * max_height;    // Random y within level height

            registry.emplace<TransformComponent>(tile, TransformComponent(glm::vec2{x_position, y_position}, glm::vec2{0.05f, 0.05f}, 0.0f));
            registry.emplace<RigidBodyComponent>(tile);
            registry.emplace<ColliderComponent>(tile, ColliderComponent(glm::vec2{1.0f, 1.0f}, true, false));
            registry.emplace<RenderComponent>(tile, hookline::get_basic_shape_debug(), true);
        }
    }
    */
    // Create a ground
    {
        auto box = registry.create();
        registry.emplace<TransformComponent>(
            box,
            TransformComponent(glm::vec2{0.0f, -0.95}, glm::vec2{4.0f, 0.05f}, 0.0f));
        registry.emplace<RigidBodyComponent>(box);
        registry.emplace<ColliderComponent>(
            box, ColliderComponent(glm::vec2{1.0f, 1.0f}, true, false));
        registry.emplace<RenderComponent>(
            box, hookline::get_basic_shape_debug(), true);
    }

    // Create grappling hook
    {
        grapple_entity = registry.create();
        registry.emplace<TransformComponent>(
            grapple_entity, TransformComponent(glm::vec2(0.0f, 0.0f),
                                               glm::vec2{0.05f, 0.05f}, 0.0f));
        registry.emplace<GrapplingHookComponent>(grapple_entity, grapple_entity,
                                                 player_.entity);
        registry.emplace<RenderComponent>(grapple_entity,
                                          hookline::get_basic_shape_debug());
    }

    // Spawn some collectables for demo
    {
    }

    // Spawn some projectiles for demo
    {
    }
}

void Game::update(float dt) {
    (void)dt;
    // Input


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
    if (player_.mouse.pressed) {
        grapple.try_attach(player_transform.position, player_.mouse.position,
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
        grapple_transform.scale = {
            glm::distance(player_transform.position, grapple.attached_position),
            0.01};
        glm::vec2 direction = glm::normalize(grapple.attached_position -
                                             player_transform.position);
        grapple_transform.rotation = -glm::atan(direction.y, direction.x);
    }
    camera.follow_player(player_transform.position.y);

    float player_height = player_transform.position.y;
    if (player_height >= next_horizontal_projectile_height) {
        projectileSystem.spawn_horizontal_projectile(true, player_height, registry);
        next_horizontal_projectile_height += 4.0f;  // Set the next trigger
    }

    // Fire a chasing projectile every 5 units of height
    if (player_height >= next_chasing_projectile_height) {
        glm::vec2 spawn_position = player_transform.position + glm::vec2(0.5f, 1.0f);
        projectileSystem.spawn_chasing_projectile(spawn_position, registry);
        next_chasing_projectile_height += 5.0f;  // Set the next trigger
    }

    // Fire a splash projectile every 3 units of height
    if (player_height >= next_splash_projectile_height) {
        glm::vec2 spawn_position = player_transform.position + glm::vec2(-0.5f, 1.0f);
        projectileSystem.spawn_spray_projectile(spawn_position, registry);
        next_splash_projectile_height += 3.0f;  // Set the next trigger
    }
    auto &health = registry.get<HealthComponent>(player_.entity);
        if (health.health <= 0) {
        // Destroy the player entity
        // TODO: This doesn't exit gracefully now
        registry.destroy(player_.entity);
        std::cout << "Player has been removed from the game due to health depletion." << std::endl;
        return;  
    }

    // System updates
    physics.update(dt, registry);
    collisions.update(dt, registry);
    collectables.update(dt, registry, player_.entity);
    projectileSystem.update(dt, registry, player_.entity);
}

void Game::render(glm::uvec2 drawable_size) {
    (void)drawable_size;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto view = registry.view<TransformComponent, RenderComponent>();
    for (auto entity : view) {
        auto &transform = view.get<TransformComponent>(entity);
        auto &renderable = view.get<RenderComponent>(entity);
        TransformComponent adjusted_transform = transform;
        adjusted_transform.position.y -= camera.y_offset;
        renderable.render(adjusted_transform);
    }
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

            glm::vec2 mouse_position = hookline::convert_mouse_to_opengl(x, y, drawable_size);
            mouse_position.y += camera.y_offset;  // Account for vertical camera offset
            player_.mouse.position = mouse_position;

        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            player_.mouse.pressed = false;
        }
    }
    return false;
}

