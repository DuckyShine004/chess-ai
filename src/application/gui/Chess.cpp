#include <iostream>

#include <SFML/Window.hpp>

#include "application/gui/Chess.hpp"

#include "engine/board/Colour.hpp"

using namespace engine;

using namespace engine::board;

using namespace engine::move;

namespace application::gui {

Chess::Chess() : _selectedSquare(nullptr) {
}

void Chess::move(sf::RenderWindow &window, Engine &engine) {
    ColourType side = engine.getSide();

    if (side == ColourType::WHITE) {
        this->getPlayerMove(window, engine, side);
    } else {
        engine.run();
    }
}

void Chess::update(sf::RenderWindow &window, Engine &engine) {
    this->move(window, engine);

    this->_board.update(engine);
}

void Chess::render(sf::RenderWindow &window) {
    this->_board.render(window);
}

Move *Chess::getPlayerMove(sf::RenderWindow &window, Engine &engine, ColourType side) {
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        return nullptr;
    }

    Square *square = this->_board.getSquare(sf::Mouse::getPosition(window));

    if (square == nullptr || square->isEmpty()) {
        return nullptr;
    }

    // If first click, then handle the first click, otherwise, handle second click
    // if (this->_isSquareSelected) {
    //     return this->handle
    // }

    this->handleFirstSelectedSquare(engine, square, side);

    return nullptr;
}

void Chess::handleFirstSelectedSquare(Engine &engine, Square *square, ColourType side) {
    this->_activeMoves.clear();

    std::vector<Move> moves = engine.generateMoves(side);

    for (Move &move : moves) {
        if (move.from != square->getSquare()) {
            continue;
        }

        this->_board.getSquare(move.to)->setIsAttacked(true);

        this->_activeMoves[move.to] = move;
    }

    this->_selectedSquare = square;
}

} // namespace application::gui
