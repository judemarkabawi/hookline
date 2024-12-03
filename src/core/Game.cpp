#include "Game.hpp"

#include <memory>

#include "core/game/GameMode.hpp"
#include "core/game/PauseMenuMode.hpp"
#include "core/game/PlayMode.hpp"
#include "core/game/StartMenuMode.hpp"

Game::Game() {
    start_menu_ = std::make_unique<StartMenuMode>();
    pause_menu_ = std::make_unique<PauseMenuMode>();
    play_game_ = std::make_unique<PlayMode>();

    current_mode_ = static_cast<GameMode *>(start_menu_.get());
}

void Game::update(float dt) { current_mode_->update(dt, *this); }

void Game::render(glm::uvec2 drawable_size) {
    current_mode_->render(drawable_size, *this);
}

bool Game::handle_event(SDL_Event const &event, glm::uvec2 drawable_size) {
    return current_mode_->handle_event(event, drawable_size, *this);
}

void Game::change_mode(GameMode::Mode mode) {
    if (mode == GameMode::Mode::StartMenuMode) {
        current_mode_ = start_menu_.get();
    } else if (mode == GameMode::Mode::PauseMenuMode) {
        current_mode_ = pause_menu_.get();
    } else {
        current_mode_ = play_game_.get();
    }
}