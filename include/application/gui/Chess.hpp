#pragma once

#include <map>

#include <SFML/Graphics.hpp>

#include "application/gui/Board.hpp"

#include "application/gui/Promotion.hpp"
#include "engine/Engine.hpp"

#include "engine/move/Move.hpp"

#include "engine/board/Colour.hpp"

namespace application::gui {

class Chess {
  public:
    Chess();

    void move(sf::RenderWindow &window, engine::Engine &engine, sf::Vector2i mousePosition);

    void update(sf::RenderWindow &window, engine::Engine &engine);

    void render(sf::RenderWindow &window);

  private:
    Board _board;

    Promotion _promotion;

    bool _isClicking;

    int _previousFrom;
    int _previousTo;

    application::gui::Square *_selectedSquare;

    std::map<int, uint16_t> _activeMoves;

    void setPreviousSquares(int from, int to);

    void clearPreviousSquares();

    void clearSelection();

    void handleFirstSelectedSquare(engine::Engine &engine, application::gui::Square *square, engine::board::ColourType side);

    void handleSecondSelectedSquare(engine::Engine &engine, application::gui::Square *square);

    bool isOwnPiece(application::gui::Square *square, engine::board::ColourType side);
};

} // namespace application::gui
