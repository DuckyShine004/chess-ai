#pragma once

#include <cstdint>

#include "application/gui/Square.hpp"

#include "engine/Engine.hpp"

namespace application::gui {

class Board {
  public:
    Board();

    Square &getSquare(sf::Vector2i mousePosition);

    void update(engine::Engine &engine);

    void render(sf::RenderWindow &window);

  private:
    Square _squares[64];
};

} // namespace application::gui