#pragma once

#include <GL/glew.h>

#include <string>
#include <unordered_map>

#include "sound/Sound.hpp"

class AssetManager {
   public:
    GLuint load_texture(const std::string &name, const std::string &filename, const GLenum texType = GL_TEXTURE_2D, const GLint filterType = GL_LINEAR_MIPMAP_LINEAR, const GLint verticalWrap = GL_REPEAT);
    GLuint get_texture(const std::string &name);

    void load_sound(const std::string &name, const std::string &filename);
    const Sound::Sample &get_sound(const std::string &name) const;

   private:
    std::unordered_map<std::string, GLuint> textures_;
    std::unordered_map<std::string, Sound::Sample> sounds_;
};
