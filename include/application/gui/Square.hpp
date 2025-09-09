#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "engine/board/Colour.hpp"

#include "application/gui/Piece.hpp"

namespace application::gui {

inline constexpr float SIZE = 100.0f;

inline constexpr float BORDER_THICKNESS = -5.0f;

inline constexpr sf::Color SQUARE_COLOURS[2] = {
    sf::Color(240, 217, 181),
    sf::Color(181, 136, 99),
};

inline constexpr sf::Color PREVIOUS_COLOURS[2] = {
    sf::Color(205, 209, 106),
    sf::Color(170, 162, 58),
};

inline constexpr sf::Color ATTACK_COLOUR = sf::Color(106, 111, 64);

inline constexpr sf::Color BORDER_COLOUR = sf::Color(0, 0, 0);

class Square {
  public:
    Square();

    Square(int rank, int file, engine::board::ColourType colour);

    application::gui::Piece &getPiece();

    bool isEmpty();

    bool isCollideWithPoint(sf::Vector2i position);

    int getSquare();

    void setIsActive(bool isActive);

    void setIsHovered(bool isHovered);

    void setIsAttacked(bool isAttacked);

    void setIsPrevious(bool isPrevious);

    bool isAttacked();

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

    bool _isActive;
    bool _isHovered;
    bool _isAttacked;

    sf::Vector2f getCentre();

    void renderAttack(sf::RenderWindow &window);

    void renderBorder(sf::RenderWindow &window);
};

} // namespace application::gui
