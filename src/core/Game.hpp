/**
 * @file Game.hpp
 *
 * Handles game state, update, rendering, etc.
 */

#pragma once

#include <SDL2/SDL.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <physics/Components.hpp>

#include "core/CameraComponent.hpp"
#include "gameplay/ProjectileSystem.hpp"
#include "gameplay/CollectableSystem.hpp"
#include "physics/CollisionSystem.hpp"
#include "physics/PhysicsSystem.hpp"

#include "gameplay/LevelData.cpp"

/**
 * Main Game class
 *
 * entt is the Entity-Component-System library we're using, any new
 * objects/things need to be made as entities with components attached to them.
 * Right now there's no spatial Scene data structure yet, we can do that soon.
 * TransformComponents in every entity and then iterating over entt views are
 * our implicit scene right now.
 */
class Game {
   public:
    Game();

    void update(float dt);
    void render(glm::uvec2 drawable_size);
    bool handle_event(SDL_Event const &event, glm::uvec2 drawable_size);
    void generate_fixed_tiles(const LevelData &level_data);

    float next_horizontal_projectile_height = 4.0f;
    float next_chasing_projectile_height = 5.0f;
    float next_splash_projectile_height = 3.0f;

    struct Player {
        entt::entity entity;
        struct Button {
            bool pressed = false;
        };
        Button up, down, left, right;
        struct Mouse {
            bool pressed = false;
            glm::vec2 position;
        } mouse;
    } player_;
    entt::entity grapple_entity;

    PhysicsSystem physics;
    CollisionSystem collisions;
    CollectableSystem collectables;
    CameraComponent camera;
    ProjectileSystem projectileSystem;
    entt::registry registry;
};