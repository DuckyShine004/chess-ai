#pragma once

#include <SFML/Graphics.hpp>

#include "application/gui/Board.hpp"

#include "engine/Engine.hpp"

#include "engine/move/Move.hpp"

#include "engine/board/Colour.hpp"

namespace application::gui {

class Chess {
  public:
    Chess();

    void move(engine::Engine &engine);

    void update(engine::Engine &engine);

    void render(sf::RenderWindow &window);

  private:
    Board _board;

    engine::move::Move getPlayerMove(engine::board::ColourType side);
};

} // namespace application::gui