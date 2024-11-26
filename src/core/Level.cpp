#include "Level.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "core/AssetManager.hpp"
#include "core/InputComponent.hpp"
#include "core/TransformComponent.hpp"
#include "gameplay/HealthComponent.hpp"
#include "gameplay/CollectableComponent.hpp"
#include "gameplay/CollectableSystem.hpp"
#include "gameplay/ProjectileSpawnerComponent.hpp"
#include "gameplay/ProjectileSystem.hpp"
#include "physics/ColliderComponent.hpp"
#include "physics/ForceComponent.hpp"
#include "physics/GrapplingHook.hpp"
#include "physics/RigidBodyComponent.hpp"
#include "render/RenderComponent.hpp"
#include "util/misc.hpp"

using json = nlohmann::json;

namespace {
/**
    Helper to make a manually defined box on the map.
 */
entt::entity create_box(entt::registry &registry, glm::vec2 position,
                        glm::vec2 scale, bool hookable, bool breakable, bool damaging, glm::vec4 color) {
    auto box = registry.create();
    registry.emplace<TransformComponent>(
        box, TransformComponent(position, scale, 0.0f));
    registry.emplace<RigidBodyComponent>(box);
    registry.emplace<ColliderComponent>(
        box, ColliderComponent().set_can_move(false).set_hookable(hookable).set_breakable(breakable).set_damaging(damaging));
    registry.emplace<RenderComponent>(
        box, RenderComponent::from_vertices_color(
                 hookline::get_basic_shape_debug(), color));
    return box;
}

entt::entity create_hookable_unbreakable(entt::registry &registry, glm::vec2 position) {
    return create_box(registry, position, glm::vec2{0.05f, 0.05f}, true, false, false, glm::vec4{0.07f, 0.11f, 0.23f, 1.0f});
}

entt::entity create_hookable_breakable(entt::registry &registry, glm::vec2 position) {
    return create_box(registry, position, glm::vec2{0.05f, 0.05f}, true, true, false, glm::vec4{0.55f, 0.27f, 0.07f, 1.0f});
}



entt::entity create_collectable(entt::registry &registry, glm::vec2 position, CollectableType type) {
    auto collectable = registry.create();
    registry.emplace<TransformComponent>(
        collectable,
        TransformComponent(position, glm::vec2{0.025f, 0.025f}, 0.0f));
    registry.emplace<CollectableComponent>(collectable, CollectableComponent{type});

    // Assign color based on collectible type
    glm::vec4 color;
    switch (type) {
        case CollectableType::Feather:
            color = {0.8f, 0.8f, 0.2f, 1.0f};  // Yellow for Feather
            break;
        case CollectableType::Potion:
            color = {0.2f, 0.8f, 0.2f, 1.0f};  // Green for Potion
            break;
        default:
            color = {0.96f, 0.48f, 0.16f, 1.0f};  // Default Orange
            break;
    }

    registry.emplace<RenderComponent>(
        collectable,
        RenderComponent::from_vertices_color(hookline::get_basic_shape_debug(), color));

    return collectable;
}

void load_spike_terrain(const json &data, Level &level) {
    auto &spike_terrain = data["map"]["spike_terrain"];
    for (const auto &entry : spike_terrain) {
        glm::vec2 position(entry[0][0].get<float>(), entry[0][1].get<float>());
        glm::vec2 scale(entry[1][0].get<float>(), entry[1][1].get<float>());
        create_box(level.registry, position, scale, false, false, true, glm::vec4{0.8f, 0.f, 0.f, 1.0f});
    }
        
}

void load_terrain(const json &data, Level &level) {
    auto &terrain = data["map"]["terrain"];
    for (const auto &entry : terrain) {
        glm::vec2 position(entry[0][0].get<float>(), entry[0][1].get<float>());
        glm::vec2 scale(entry[1][0].get<float>(), entry[1][1].get<float>());
        create_box(level.registry, position, scale, false, false, false, glm::vec4{0.2f, 0.2f, 0.2f, 1.0f});
    }
        
}

void load_map(const json &data, Level &level) {
    // Load terrain
    load_terrain(data, level);
    load_spike_terrain(data, level);

    // Hookable unbreakable objects
    auto &hookable_unbreakable = data["map"]["hookable_unbreakable"];
    for (const auto &entry : hookable_unbreakable) {
        glm::vec2 position(entry[0][0].get<float>(), entry[0][1].get<float>());
        glm::vec2 scale(entry[1][0].get<float>(), entry[1][1].get<float>());
        create_box(level.registry, position, scale, true, false, false, glm::vec4{0.07f, 0.11f, 0.23f, 1.0f});
    }

    // Hookable breakable objects
    auto &hookable_breakable = data["map"]["hookable_breakable"];
    for (const auto &entry : hookable_breakable) {
        glm::vec2 position(entry[0][0].get<float>(), entry[0][1].get<float>());
        glm::vec2 scale(entry[1][0].get<float>(), entry[1][1].get<float>());
        create_box(level.registry, position, scale, true, true, false, glm::vec4{0.55f, 0.27f, 0.07f, 1.0f});
    }
}
void load_projectiles(const json &data, Level &level) {
    auto &projectiles = data["projectiles"];

    auto create_projectile_spawner = [&](SpawnerType type, const json &entry) {
        glm::vec2 position(entry[0][0].get<float>(), entry[0][1].get<float>()); // Transform position
        glm::vec2 position_range(entry[1][0].get<float>(), entry[1][1].get<float>()); // Range (horizontal/vertical)
        float interval = entry[2].get<float>();
        float small_interval = entry[3].get<float>();
        int burst_count = entry[4].get<int>();
        float projectile_speed = entry[5].get<float>();
        float projectile_lifetime = entry[6].get<float>();
        int projectile_count = (type == SpawnerType::Spray) ? entry[7].get<int>() : 1;
        float trigger_height = (type == SpawnerType::Chasing || type == SpawnerType::Spray) ? entry[8].get<float>() : 0;

        auto spawner = level.registry.create();
        level.registry.emplace<ProjectileSpawnerComponent>(
            spawner, type, position_range, interval, small_interval, burst_count,
            projectile_speed, projectile_lifetime, projectile_count, trigger_height);
        level.registry.emplace<TransformComponent>(
            spawner, position, glm::vec2{0.0f, 0.0f});
    };

    if (projectiles.contains("Horizontal")) {
        for (const auto &entry : projectiles["Horizontal"]) {
            create_projectile_spawner(SpawnerType::Horizontal, entry);
        }
    }

    if (projectiles.contains("Vertical")) {
        for (const auto &entry : projectiles["Vertical"]) {
            create_projectile_spawner(SpawnerType::Vertical, entry);
        }
    }

    if (projectiles.contains("Chasing")) {
        for (const auto &entry : projectiles["Chasing"]) {
            create_projectile_spawner(SpawnerType::Chasing, entry);
        }
    }

    if (projectiles.contains("Spray")) {
        for (const auto &entry : projectiles["Spray"]) {
            create_projectile_spawner(SpawnerType::Spray, entry);
        }
    }
}


void load_player(const json &data, Level &level) {
    auto position = data["player"]["position"];
    assert(position.size() == 2 && "Invalid position in JSON");

    auto scale = data["player"]["scale"];
    assert(scale.size() == 2 && "Invalid scale in JSON");

    auto color = data["player"]["color"];
    assert(color.size() == 4 && "Invalid color in JSON");

    int health = data["player"]["health"];
    assert(health >= 1 && "Invalid health in JSON");

    glm::vec2 position_vec(position[0].get<float>(), position[1].get<float>());
    glm::vec2 scale_vec(scale[0].get<float>(), scale[1].get<float>());
    glm::vec4 color_vec(color[0].get<float>(), color[1].get<float>(),
                        color[2].get<float>(), color[3].get<float>());

    entt::registry &registry = level.registry;
    auto player = registry.create();
    registry.emplace<TransformComponent>(
        player, TransformComponent(position_vec, scale_vec, 0.0f));
    registry.emplace<RigidBodyComponent>(player);
    registry.emplace<ForceComponent>(player);
    registry.emplace<ColliderComponent>(
        player, ColliderComponent().set_hookable(false));
    registry.emplace<RenderComponent>(
        player, RenderComponent::from_vertices_color(
                    hookline::get_basic_shape_debug(), color_vec));
    registry.emplace<InputComponent>(player);
    registry.emplace<HealthComponent>(player, HealthComponent(health));
    level.player = player;
}

void load_sounds(const json &data, Level &level) {
    std::filesystem::path sound_assets_dir = "../../assets/sounds";

    std::vector<std::string> sounds = data["sounds"];
    for (auto &sound : sounds) {
        std::filesystem::path sound_path = sound;
        std::filesystem::path file_path = sound_assets_dir / sound_path;
        level.assets.load_sound(sound_path.stem().string(),
                                hookline::data_path(file_path.string()));
    }
}

void load_collectables(const json &data, Level &level) {
    const auto &collectables = data["collectables"];

    // Feathers
    for (const auto &pos : collectables["Feather"]) {
        glm::vec2 position(pos[0].get<float>(), pos[1].get<float>());
        create_collectable(level.registry, position, CollectableType::Feather);
    }

    // Potions
    for (const auto &pos : collectables["Potion"]) {
        glm::vec2 position(pos[0].get<float>(), pos[1].get<float>());
        create_collectable(level.registry, position, CollectableType::Potion);
    }
}




}  // namespace

Level Level::load_json(const std::string &filename) {
    std::cout << "----------" << "\n";
    std::cout << "Loading level: " << filename << "\n";

    std::ifstream file(filename);
    json data = json::parse(file);

    Level level;

    std::cout << "Loading collectables..." << "\n";
    load_collectables(data, level);

    std::cout << "Loading map..." << "\n";
    load_map(data, level);
    

    std::cout << "Loading player..." << "\n";
    load_player(data, level);

    std::cout << "loading projectiles..." << "\n";
    load_projectiles(data, level);

    std::cout << "Loading sounds..." << "\n";
    load_sounds(data, level);

    std::cout << "Loading camera, grapple hook, etc..." << "\n";
    // Create grappling hook
    {
        auto grapple_entity = level.registry.create();
        level.registry.emplace<TransformComponent>(
            grapple_entity, TransformComponent(glm::vec2(0.0f, 0.0f),
                                               glm::vec2{0.05f, 0.05f}, 0.0f));
        level.registry.emplace<GrapplingHookComponent>(
            grapple_entity, grapple_entity, level.player);
        level.registry.emplace<RenderComponent>(
            grapple_entity, RenderComponent::from_vertices_color(
                                hookline::get_basic_shape_debug()));
        level.grapple = grapple_entity;
    }

    std::cout << "Level loaded" << "\n";
    std::cout << "----------" << "\n";
    return level;
}