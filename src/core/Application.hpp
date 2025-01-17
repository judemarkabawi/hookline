/**
 * The Application handles making the window and running the event/game loop.
 */

#pragma once
#include <SDL2/SDL.h>

#include <memory>

#include "Game.hpp"

class Application {
   public:
    bool init();
    void run();

   private:
    SDL_Window* window_;
    SDL_GLContext context_;
    std::unique_ptr<Game> game_;
    bool quit_ = false;
};