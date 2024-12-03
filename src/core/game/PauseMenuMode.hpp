#pragma once

#include <SDL2/SDL.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/game/GameMode.hpp"
#include "render/RenderSystem.hpp"

class Game;

class PauseMenuMode : public GameMode {
   public:
    PauseMenuMode();

    void update(float dt, Game& game) override;
    void render(glm::uvec2 drawable_size, Game& game) override;
    bool handle_event(SDL_Event const& event, glm::uvec2 drawable_size,
                      Game& game) override;

   private:
    RenderSystem rendering_;
    entt::registry registry_;  // for text components, overkill
};