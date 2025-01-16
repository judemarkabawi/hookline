#include "TextComponent.hpp"

#include "shader/TextShader.hpp"
#include "util/misc.hpp"

TextComponent TextComponent::from_text(std::string text, glm::vec2 position,
                                       float scale) {
    auto shape = hookline::get_basic_shape_debug();
    Mesh2D mesh =
        Mesh2D::from_verts_texture(shape, {{0, 0}, {1, 0}, {0, 1}, {1, 1}});

    return TextComponent{
        .position = position,
        .scale = scale,
        .text = std::move(text),
        .shader = TextShader(),
        .mesh = std::move(mesh),
    };
}

float get_centered_x_position(const glm::uvec2 drawable_size,
                              const std::vector<Glyph>& glyphs, float scale) {
    float screen_width = drawable_size.x;
    float center_x = screen_width / 2.0f;

    float text_width = 0;
    for (auto const& ch : glyphs) {
        text_width += ch.advance * scale;
    }

    float start_position = center_x - (text_width / 2.0f);
    return start_position;
}