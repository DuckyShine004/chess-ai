#include <SFML/Window.hpp>

#include "application/gui/Chess.hpp"

#include "engine/board/Piece.hpp"

using namespace engine;

using namespace engine::board;

using namespace engine::move;

namespace application::gui {

Chess::Chess() {
}

void Chess::move(Engine &engine) {
    ColourType side = engine.getSide();

    if (side == ColourType::WHITE) {
        this->getPlayerMove(side);
    } else {
        engine.run();
    }
}

void Chess::update(Engine &engine) {
    this->move(engine);

    this->_board.update(engine);
}

void Chess::render(sf::RenderWindow &window) {
    this->_board.render(window);
}

Move Chess::getPlayerMove(ColourType side) {
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        return Move();
    }

    // Check if valid square
    Square &square = this->_board.getSquare(sf::Mouse::getPosition());

    if (square.isEmpty()) {
        return Move();
    }

    return Move();
}

} // namespace application::gui