#include "Level.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "core/AssetManager.hpp"
#include "core/InputComponent.hpp"
#include "core/TransformComponent.hpp"
#include "gameplay/HealthComponent.hpp"
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
                        glm::vec2 scale, bool hookable, glm::vec4 color, 
                        RenderComponent::RenderType type = RenderComponent::RenderType::BASE) {
    auto box = registry.create();
    registry.emplace<TransformComponent>(
        box, TransformComponent(position, scale, 0.0f));
    registry.emplace<RigidBodyComponent>(box);
    registry.emplace<ColliderComponent>(
        box, ColliderComponent().set_can_move(false).set_hookable(hookable));
    registry.emplace<RenderComponent>(
        box, RenderComponent::from_vertices_color_tex(
                 hookline::get_basic_shape_debug(), color, hookline::get_basic_uvs_debug(), type));
    return box;
}

void load_objects(const json &data, Level &level) {
    auto &objects = data["map"]["objects"];
    for (auto &object : objects) {
        auto position = object["position"];
        assert(position.size() == 2 && "Invalid position in JSON");

        auto scale = object["scale"];
        assert(scale.size() == 2 && "Invalid scale in JSON");

        auto color = object["color"];
        assert(color.size() == 4 && "Invalid color in JSON");

        bool hookable = object["hookable"];

        glm::vec2 position_vec(position[0].get<float>(),
                               position[1].get<float>());
        glm::vec2 scale_vec(scale[0].get<float>(), scale[1].get<float>());
        glm::vec4 color_vec(color[0].get<float>(), color[1].get<float>(),
                            color[2].get<float>(), color[3].get<float>());

        RenderComponent::RenderType type = RenderComponent::RenderType::BASE;
        if(hookable) {
            type = RenderComponent::RenderType::GRAPPLE_POINT;
        } 
        create_box(level.registry, position_vec, scale_vec, hookable,
                   color_vec, type);
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
        player, RenderComponent::from_vertices_color_tex(
                    hookline::get_basic_shape_debug(), color_vec, 
                    hookline::get_basic_uvs_debug(), RenderComponent::RenderType::PLAYER));
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

}  // namespace

Level Level::load_json(const std::string &filename) {
    std::cout << "----------" << "\n";
    std::cout << "Loading level: " << filename << "\n";

    std::ifstream file(filename);
    json data = json::parse(file);

    Level level;

    std::cout << "Loading objects..." << "\n";
    load_objects(data, level);

    std::cout << "Loading player..." << "\n";
    load_player(data, level);

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