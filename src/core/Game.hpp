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
#include "gameplay/CollectableSystem.hpp"
#include "gameplay/ProjectileSystem.hpp"
#include "physics/CollisionSystem.hpp"
#include "physics/PhysicsSystem.hpp"
#include "render/CameraSystem.hpp"
#include "render/RenderSystem.hpp"

/**
 * Main Game class
 */
class Game {
   public:
    Game();

    void update(float dt);
    void render(glm::uvec2 drawable_size);
    bool handle_event(SDL_Event const &event, glm::uvec2 drawable_size);

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

    enum class Mode {
        StartMenu,
        Game,
    } mode = Mode::StartMenu;

   private:
    void setup_camera();
    void setup_map();

    /* -- TODO: Make these Modes and not just in Game -- */
    // Start Menu "Mode"
    RenderSystem start_menu_rendering;

    entt::registry start_menu_registry;  // for text components, overkill
    void start_menu_update(float dt);
    void start_menu_render(glm::uvec2 drawable_size);
    bool start_menu_handle_event(SDL_Event const &event,
                                 glm::uvec2 drawable_size);
};