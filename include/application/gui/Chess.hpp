#pragma once

#include <map>

#include <SFML/Graphics.hpp>

#include "application/gui/Board.hpp"

#include "engine/Engine.hpp"

#include "engine/move/Move.hpp"

#include "engine/board/Colour.hpp"

namespace application::gui {

class Chess {
  public:
    Chess();

    void move(sf::RenderWindow &window, engine::Engine &engine);

    void update(sf::RenderWindow &window, engine::Engine &engine);

    void render(sf::RenderWindow &window);

  private:
    Board _board;

    application::gui::Square *_selectedSquare;

    std::map<int, engine::move::Move> _activeMoves;

    engine::move::Move *getPlayerMove(sf::RenderWindow &window, engine::Engine &engine, engine::board::ColourType side);

    void handleFirstSelectedSquare(engine::Engine &engine, application::gui::Square *square, engine::board::ColourType side);

    // engine::board::Move *handleSecondSelectedSquare();
};

} // namespace application::gui
