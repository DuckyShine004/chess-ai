#pragma once

#include <SFML/Graphics.hpp>

#include "engine/board/Colour.hpp"

#include "application/gui/Piece.hpp"

namespace application::gui {

inline constexpr float SIZE = 100.0f;

inline constexpr sf::Color COLOURS[2] = {
    sf::Color(240, 217, 181),
    sf::Color(181, 136, 99),
};

class Square {
  public:
    Square();

    Square(int rank, int file, engine::board::ColourType colour);

    application::gui::Piece &getPiece();

    bool isEmpty();

    void render(sf::RenderWindow &window);

  private:
    sf::RectangleShape _square;

    engine::board::ColourType _colour;

    Piece _piece;

    float _file;
    float _rank;
    float _size;

    float _x;
    float _y;
};

} // namespace application::gui