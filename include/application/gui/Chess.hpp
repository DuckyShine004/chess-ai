#pragma once

#include <SFML/Graphics.hpp>

#include "application/gui/Board.hpp"

namespace application::gui {

class Chess {
  public:
    Chess();

    void update();

    void render(sf::RenderWindow &window);

  private:
    Board _board;
};

} // namespace application::gui