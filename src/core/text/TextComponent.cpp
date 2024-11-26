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