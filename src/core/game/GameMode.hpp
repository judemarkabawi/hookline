/**
 * GameMode represents different states the game is in, mainly Paused modes in
 * menus and the main game PlayMode.
 */
#pragma once

#include <SDL2/SDL.h>

#include <glm/glm.hpp>

class Game;

class GameMode {
   public:
    enum class Mode {
        StartMenuMode,
        PauseMenuMode,
        PlayMode,
        GameOverMenuMode,
        WinMenuMode
    };

    virtual ~GameMode() = default;
    virtual void update(float dt, Game& game) = 0;
    virtual void render(glm::uvec2 drawable_size, Game& game) = 0;
    virtual bool handle_event(SDL_Event const& event, glm::uvec2 drawable_size,
                              Game& game) = 0;
};
