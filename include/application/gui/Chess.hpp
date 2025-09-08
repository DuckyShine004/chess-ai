#pragma once

#include <SFML/Graphics.hpp>

#include "application/gui/Board.hpp"

#include "engine/board/Colour.hpp"

namespace application::gui {

class Chess {
  public:
    Chess();

    void move(engine::board::ColourType side);

    void update();

    void render(sf::RenderWindow &window);

  private:
    Board _board;
};

} // namespace application::gui