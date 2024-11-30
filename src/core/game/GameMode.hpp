#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

class GameMode {
public:
    enum class Mode {
        StartMenuMode,
        PlayMode,
    };

    virtual ~GameMode() = default;
    virtual void update(float dt) = 0;
    virtual void render(glm::uvec2 drawable_size) = 0;
    virtual bool handle_event(SDL_Event const &event, glm::uvec2 drawable_size) = 0;
};
