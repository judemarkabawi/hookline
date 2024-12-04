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
        GameOverMenuMode
    };

    virtual ~GameMode() = default;
    virtual void update(float dt, Game& game) = 0;
    virtual void render(glm::uvec2 drawable_size, Game& game) = 0;
    virtual bool handle_event(SDL_Event const& event, glm::uvec2 drawable_size,
                              Game& game) = 0;
};
