#include "PauseMenuMode.hpp"

#include <SDL_events.h>
#include <SDL_keycode.h>

#include "core/Game.hpp"
#include "core/text/TextComponent.hpp"

PauseMenuMode::PauseMenuMode() {
    entt::entity title = registry_.create();
    registry_.emplace<TextComponent>(
        title, TextComponent::from_text("Paused", {-0.35, 0.5}, 5.0f));

    entt::entity play_button = registry_.create();
    registry_.emplace<TextComponent>(
        play_button,
        TextComponent::from_text("Press [Esc] to resume or [Q] to quit",
                                 {-0.75, -0.1}, 1.0f));
}

void PauseMenuMode::update(float dt, Game& _) {
    (void)dt;
    return;
}

void PauseMenuMode::render(glm::uvec2 drawable_size, Game& _) {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    rendering_.render_menu_background(drawable_size);

    rendering_.render_text(drawable_size, registry_);
}

bool PauseMenuMode::handle_event(SDL_Event const& event,
                                 glm::uvec2 drawable_size, Game& game) {
    (void)drawable_size;

    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            // Return to game
            game.change_mode(GameMode::Mode::PlayMode);
            return true;
        } else if (event.key.keysym.sym == SDLK_q) {
            // Quit game
            exit(0);
            return true;
        }
    }
    return false;
}