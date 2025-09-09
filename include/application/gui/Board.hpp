#pragma once

#include "application/gui/Square.hpp"

#include "engine/Engine.hpp"

namespace application::gui {

class Board {
  public:
    Board();

    Square *getSquare(int square);

    Square *getSquare(sf::Vector2i mousePosition);

    void update(sf::RenderWindow &window, engine::Engine &engine);

    void render(sf::RenderWindow &window);

  private:
    Square *_squares[64];

    Square *_hoveredSquare;

    float _x;
    float _y;

    void onHover(sf::RenderWindow &window);
};

} // namespace application::gui
