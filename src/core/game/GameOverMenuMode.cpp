#include "GameOverMenuMode.hpp"

#include <SDL_events.h>
#include <SDL_keycode.h>

#include "core/Game.hpp"
#include "core/game/StartMenuMode.hpp"
#include "core/text/TextComponent.hpp"

GameOverMenuMode::GameOverMenuMode() {
    entt::entity title = registry_.create();
    registry_.emplace<TextComponent>(
        title, TextComponent::from_text("Game Over", {0.0, 0.5}, 3.0f));

    entt::entity play_button = registry_.create();
    registry_.emplace<TextComponent>(
        play_button, TextComponent::from_text("Click anywhere to restart game",
                                              {0.0, -0.1}, 1.0f));
}

void GameOverMenuMode::update(float dt, Game& _) {
    (void)dt;
    return;
}

void GameOverMenuMode::render(glm::uvec2 drawable_size, Game& _) {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    rendering_.render_menu_background(drawable_size);

    rendering_.render_text(drawable_size, registry_);
}

bool GameOverMenuMode::handle_event(SDL_Event const& event,
                                    glm::uvec2 drawable_size, Game& _) {
    (void)drawable_size;

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            // Quit game
            exit(0);
        }
    }
    return false;
}