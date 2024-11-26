#pragma once

#include <glm/glm.hpp>
#include <string>

#include "render/Mesh2D.hpp"
#include "shader/TextShader.hpp"

struct TextComponent {
    glm::vec2 position;
    float scale;
    std::string text;
    TextShader shader;
    Mesh2D mesh;

    static TextComponent from_text(std::string text, glm::vec2 position,
                                   float scale = 1.0f);
};
