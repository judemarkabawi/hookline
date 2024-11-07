#include "RenderComponent.hpp"

#include "core/TransformComponent.hpp"
#include "gameplay/ProjectileComponent.hpp"

RenderComponent::RenderComponent(std::vector<glm::vec2>&& vertices,
                                 bool visible)
    : vao_(0), vbo_(0), verts_(std::move(vertices)), visible_(visible) {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
}

RenderComponent::~RenderComponent() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void RenderComponent::render(TransformComponent transform) {
    if (!visible_) {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, verts_.size() * sizeof(verts_[0]),
                 verts_.data(), GL_STATIC_DRAW);

    glBindVertexArray(vao_);
    glVertexAttribPointer(program_.position_loc, 2, GL_FLOAT, GL_FALSE, 0,
                          nullptr);
    glEnableVertexAttribArray(program_.position_loc);

    glUseProgram(program_.program);

    // Set uniforms
    glUniform2f(program_.u_position_loc, transform.position.x,
                transform.position.y);
    glUniform2f(program_.u_scale_loc, transform.scale.x, transform.scale.y);
    glUniform1f(program_.u_rotation_loc, transform.rotation);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, verts_.size());

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderComponent::set_visible(bool visible) { visible_ = visible; }