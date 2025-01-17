/**
 * Level contains all the data for a single level of the game, including loaded
 * assets, the player, obstacles, etc.
 */

#pragma once

#include <entt/entt.hpp>

#include "core/AssetManager.hpp"

struct Level {
    AssetManager assets;
    entt::registry registry;
    entt::entity player;
    entt::entity grapple;

    /**
     * Loads a level from a JSON file.
     *
     * Reads required assets and loads them. Populates a new entt::registry with
     * all the objects with all their components, etc.
     */
    static Level load_json(const std::string& filename);
};
