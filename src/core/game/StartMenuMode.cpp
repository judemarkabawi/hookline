#include "StartMenuMode.hpp"

#include "core/Game.hpp"
#include "core/game/GameMode.hpp"
#include "core/text/TextComponent.hpp"

StartMenuMode::StartMenuMode() {
    entt::entity title = registry_.create();
    registry_.emplace<TextComponent>(
        title, TextComponent::from_text("Hookline", {0.0f, 0.5}, 3.0f));

    entt::entity play_button = registry_.create();
    registry_.emplace<TextComponent>(
        play_button,
        TextComponent::from_text("Click anywhere to play", {0.0f, -0.1}, 1.0f));
}

void StartMenuMode::update(float dt, Game& _) {
    (void)dt;
    return;
}

void StartMenuMode::render(glm::uvec2 drawable_size, Game& _) {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    rendering_.render_menu_background(drawable_size);

    rendering_.render_text(drawable_size, registry_);
}

bool StartMenuMode::handle_event(SDL_Event const& event,
                                 glm::uvec2 drawable_size, Game& game) {
    (void)drawable_size;

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            game.change_mode(GameMode::Mode::PlayMode);
            return true;
        }
    }
    return false;
}