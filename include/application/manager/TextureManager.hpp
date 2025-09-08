#pragma once

#include <SFML/Graphics.hpp>

#include "application/manager/Manager.hpp"

namespace application::manager {

inline constexpr const char *TEXTURE_PATH = "resources/textures/pieces/";

// clang-format off
inline constexpr const char *TEXTURES[12] = {
    "white-pawn.png",
    "white-knight.png",
    "white-bishop.png",
    "white-rook.png",
    "white-queen.png",
    "white-king.png",
    "black-pawn.png",
    "black-knight.png",
    "black-bishop.png",
    "black-rook.png",
    "black-queen.png",
    "black-king.png",
};
// clang-format on

class TextureManager final : public Manager {
  public:
    static TextureManager &getInstance();

    sf::Texture &getTexture(int id);

    void initialise() override;

  private:
    TextureManager();

    ~TextureManager();

    sf::Texture _textures[12];
};

} // namespace application::manager