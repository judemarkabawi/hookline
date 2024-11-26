#include "AssetManager.hpp"

#include <stdexcept>

#include "util/load_save_png.hpp"

//only need first 2 params, default is 2D tex, w vertical wrap and linear mipmap filtering
GLuint AssetManager::load_texture(const std::string &name,
                                const std::string &filename,
                                const GLenum texType, 
                                const GLint filterType,
                                const GLint verticalWrap) {
    // Load image
    glm::uvec2 size;
    std::vector<glm::u8vec4> data;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(texType, texture);

    if(texType == GL_TEXTURE_CUBE_MAP) {
        load_png(filename + "posx.png", &size, &data, OriginLocation::LowerLeftOrigin);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data.data());
        load_png(filename + "posy.png", &size, &data, OriginLocation::LowerLeftOrigin);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data.data());
        load_png(filename + "posz.png", &size, &data, OriginLocation::LowerLeftOrigin);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data.data());
        load_png(filename + "negx.png", &size, &data, OriginLocation::LowerLeftOrigin);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data.data());
        load_png(filename + "negy.png", &size, &data, OriginLocation::LowerLeftOrigin);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data.data());
        load_png(filename + "negz.png", &size, &data, OriginLocation::LowerLeftOrigin);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data.data());
    } else {
        load_png(filename, &size, &data, OriginLocation::LowerLeftOrigin);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data.data());
    }

    if(texType == GL_TEXTURE_CUBE_MAP) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    } else {
        if(verticalWrap == GL_REPEAT) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        } else if (GL_CLAMP) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        }
        if(filterType == GL_LINEAR_MIPMAP_LINEAR) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        } else if (filterType == GL_NEAREST) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
        }
    }
    if(filterType != GL_LINEAR_MIPMAP_LINEAR) glGenerateMipmap(GL_TEXTURE_2D);

    textures_[name] = texture;
    return texture;
}

GLuint AssetManager::get_texture(const std::string &name) {
    auto texture = textures_.find(name);
    if (texture == textures_.end()) {
        throw std::runtime_error("Invalid texture name: " + name);
    }

    return texture->second;
}

void AssetManager::load_sound(const std::string &name,
                              const std::string &filename) {
    Sound::Sample sound(filename);
    sounds_.insert_or_assign(name, std::move(sound));
}

const Sound::Sample &AssetManager::get_sound(const std::string &name) const {
    auto sound = sounds_.find(name);
    if (sound == sounds_.end()) {
        throw std::runtime_error("Invalid sound name: " + name);
    }

    return sound->second;
}