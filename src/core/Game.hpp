#pragma once

#include <SDL2/SDL.h>

#include <glm/glm.hpp>

#include "core/game/PlayMode.hpp"
#include "core/game/StartMenuMode.hpp"

class Game {
   public:
    Game();

    void update(float dt);
    void render(glm::uvec2 drawable_size);
    bool handle_event(SDL_Event const &event, glm::uvec2 drawable_size);

   private:
    void change_mode(GameMode::Mode mode);

    GameMode *current_mode_;
    std::unique_ptr<StartMenuMode> start_menu_;
    std::unique_ptr<PlayMode> play_game_;
};