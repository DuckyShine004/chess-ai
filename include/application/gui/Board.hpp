#pragma once

#include "application/gui/Square.hpp"

namespace application::gui {

class Board {
  public:
    Board();

    void render(sf::RenderWindow &window);

  private:
    Square _squares[64];
};

} // namespace application::gui