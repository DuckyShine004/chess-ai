#pragma once

#include <SFML/Graphics.hpp>

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

namespace application::gui {

class Piece {
  public:
    Piece();

    Piece(engine::board::PieceType piece, engine::board::ColourType colour);

    void render(sf::RenderWindow &window);

  private:
    engine::board::PieceType _piece;

    engine::board::ColourType _colour;
};

} // namespace application::gui
