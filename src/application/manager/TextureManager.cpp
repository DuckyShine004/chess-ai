#include <stdexcept>

#include "application/manager/TextureManager.hpp"

namespace application::manager {

TextureManager::TextureManager() = default;

TextureManager::~TextureManager() = default;

TextureManager &TextureManager::getInstance() {
    static TextureManager instance;

    return instance;
}

void TextureManager::initialise() {
    for (int id = 0; id < 12; ++id) {
        std::string path = std::string(TEXTURE_PATH) + TEXTURES[id];

        if (!this->_textures[id].loadFromFile(path)) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
    }
}

sf::Texture &TextureManager::getTexture(int id) {
    return this->_textures[id];
}

} // namespace application::manager