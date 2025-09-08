#pragma once

#include <SFML/Graphics.hpp>

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

namespace application::gui {

class Piece {
  public:
    Piece();

    Piece(engine::board::PieceType piece, engine::board::ColourType colour);

    void setPiece(engine::board::PieceType piece);

    void setColour(engine::board::ColourType colour);

    bool isEmpty();

    void clear();

    void render(float x, float y, float squareSize, sf::RenderWindow &window);

  private:
    engine::board::PieceType _piece;

    engine::board::ColourType _colour;

    sf::Sprite getSprite(float x, float y, float squareSize);
};

} // namespace application::gui
