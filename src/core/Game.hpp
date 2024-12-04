#pragma once

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <memory>

#include "core/game/GameMode.hpp"
#include "core/game/PauseMenuMode.hpp"
#include "core/game/PlayMode.hpp"
#include "core/game/StartMenuMode.hpp"
#include "core/game/GameOverMenuMode.hpp"
#include "core/game/WinMenuMode.hpp"

class Game {
   public:
    Game();

    void update(float dt);
    void render(glm::uvec2 drawable_size);
    bool handle_event(SDL_Event const &event, glm::uvec2 drawable_size);

    void change_mode(GameMode::Mode mode);

   private:
    GameMode *current_mode_;
    std::unique_ptr<StartMenuMode> start_menu_;
    std::unique_ptr<PauseMenuMode> pause_menu_;
    std::unique_ptr<PlayMode> play_game_;
    std::unique_ptr<GameOverMenuMode> game_over_menu_;
    std::unique_ptr<WinMenuMode> win_menu_;
};