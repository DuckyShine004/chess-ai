#pragma once

#include "application/gui/Square.hpp"

#include "engine/Engine.hpp"

namespace application::gui {

class Board {
  public:
    Board();

    Square *getSquare(int square);

    Square *getSquare(sf::Vector2i mousePosition);

    void update(engine::Engine &engine);

    void render(sf::RenderWindow &window);

  private:
    Square *_squares[64];

    float _x;
    float _y;
};

} // namespace application::gui
