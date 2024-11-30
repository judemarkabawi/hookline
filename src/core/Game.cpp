#include "Game.hpp"

#include <memory>

#include "core/game/GameMode.hpp"
#include "core/game/PlayMode.hpp"
#include "core/game/StartMenuMode.hpp"

Game::Game() {
    start_menu_ = std::make_unique<StartMenuMode>(
        [this](GameMode::Mode mode) { change_mode(mode); });
    play_game_ = std::make_unique<PlayMode>(
        [this](GameMode::Mode mode) { change_mode(mode); });

    current_mode_ = static_cast<GameMode *>(start_menu_.get());
}

void Game::update(float dt) { current_mode_->update(dt); }

void Game::render(glm::uvec2 drawable_size) {
    current_mode_->render(drawable_size);
}

bool Game::handle_event(SDL_Event const &event, glm::uvec2 drawable_size) {
    return current_mode_->handle_event(event, drawable_size);
}

void Game::change_mode(GameMode::Mode mode) {
    if (mode == GameMode::Mode::StartMenuMode) {
        current_mode_ = start_menu_.get();
    } else {
        current_mode_ = play_game_.get();
    }
}