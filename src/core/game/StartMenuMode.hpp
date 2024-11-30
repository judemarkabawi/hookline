#pragma once

#include <SDL2/SDL.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/game/GameMode.hpp"
#include "render/RenderSystem.hpp"

class Game;

class StartMenuMode : public GameMode {
   public:
    StartMenuMode(std::function<void(GameMode::Mode)> change_mode_callback_);

    void update(float dt) override;
    void render(glm::uvec2 drawable_size) override;
    bool handle_event(SDL_Event const &event, glm::uvec2 drawable_size) override;

   private:
    RenderSystem rendering_;
    entt::registry registry_;  // for text components, overkill
    std::function<void(GameMode::Mode)> change_mode_callback_;
};