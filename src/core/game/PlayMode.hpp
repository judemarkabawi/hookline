/**
 * @file Game.hpp
 *
 * Handles game state, update, rendering, etc.
 */

#pragma once

#include <SDL2/SDL.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/AssetManager.hpp"
#include "core/game/GameMode.hpp"
#include "gameplay/CollectableSystem.hpp"
#include "gameplay/ProjectileSystem.hpp"
#include "physics/CollisionSystem.hpp"
#include "physics/PhysicsSystem.hpp"
#include "render/CameraSystem.hpp"
#include "render/RenderSystem.hpp"

/**
 * Main Game class
 */
class PlayMode : public GameMode {
   public:
    PlayMode();

    void update(float dt, Game& game) override;
    void render(glm::uvec2 drawable_size, Game& game) override;
    bool handle_event(SDL_Event const& event, glm::uvec2 drawable_size,
                      Game& game) override;

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
    entt::entity camera_entity;

    AssetManager asset_manager;
    RenderSystem rendering;
    std::unique_ptr<CameraSystem> camera_system;
    PhysicsSystem physics;
    CollisionSystem collisions;
    CollectableSystem collectables;
    ProjectileSystem projectileSystem;
    entt::registry registry;

   private:
    void setup_camera();
    void setup_map();
};