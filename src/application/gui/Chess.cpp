#include <SFML/Window.hpp>

#include "application/gui/Chess.hpp"

#include "engine/board/Colour.hpp"

using namespace engine;

using namespace engine::board;

using namespace engine::move;

namespace application::gui {

Chess::Chess() : _isClicking(false), _selectedSquare(nullptr), _previousFrom(-1), _previousTo(-1) {
}

void Chess::move(sf::RenderWindow &window, Engine &engine, sf::Vector2i mousePosition) {
    ColourType side = engine.getSide();

    if (side != ColourType::WHITE) {
        return;
    }

    Square *square = this->_board.getSquare(mousePosition);

    if (square == nullptr) {
        this->clearSelection();

        return;
    }

    if (square != this->_selectedSquare && this->isOwnPiece(square, side)) {
        this->clearSelection();
    }

    if (_selectedSquare == nullptr) {
        this->handleFirstSelectedSquare(engine, square, side);
    } else {
        this->handleSecondSelectedSquare(engine, square);
    }
}

void Chess::update(sf::RenderWindow &window, Engine &engine) {
    if (engine.getSide() != ColourType::WHITE) {
        uint16_t &move = engine.getMove();

        int from = Move::getFrom(move);
        int to = Move::getTo(move);

        this->setPreviousSquares(from, to);

        engine.makeMove(move);
    }

    this->_board.update(window, engine);
}

void Chess::render(sf::RenderWindow &window) {
    this->_board.render(window);
}

void Chess::setPreviousSquares(int from, int to) {
    this->clearPreviousSquares();

    this->_board.getSquare(from)->setIsPrevious(true);
    this->_board.getSquare(to)->setIsPrevious(true);

    this->_previousFrom = from;
    this->_previousTo = to;
}

void Chess::clearPreviousSquares() {
    if (this->_previousFrom == -1 || this->_previousTo == -1) {
        return;
    }

    this->_board.getSquare(this->_previousFrom)->setIsPrevious(false);
    this->_board.getSquare(this->_previousTo)->setIsPrevious(false);
}

void Chess::clearSelection() {
    for (const auto &[square, move] : this->_activeMoves) {
        int to = Move::getTo(move);

        this->_board.getSquare(to)->setIsAttacked(false);
    }

    this->_activeMoves.clear();

    if (this->_selectedSquare != nullptr) {
        this->_selectedSquare->setIsActive(false);

        this->_selectedSquare = nullptr;
    }
}

void Chess::handleFirstSelectedSquare(Engine &engine, Square *square, ColourType side) {
    if (square->isEmpty()) {
        return;
    }

    if (square == this->_selectedSquare) {
        this->clearSelection();

        return;
    }

    Move::MoveList moves = engine.generateMoves(side);

    for (int i = 0; i < moves.size; ++i) {
        uint16_t &move = moves.moves[i];

        if (!engine.isMoveLegal(move, side)) {
            continue;
        }

        int from = Move::getFrom(move);
        int to = Move::getTo(move);

        if (from != square->getSquare()) {
            continue;
        }

        this->_board.getSquare(to)->setIsAttacked(true);

        this->_activeMoves[to] = move;
    }

    if (!this->_activeMoves.empty()) {
        this->_selectedSquare = square;

        this->_selectedSquare->setIsActive(true);
    }
}

void Chess::handleSecondSelectedSquare(Engine &engine, Square *square) {
    if (!square->isAttacked()) {
        this->clearSelection();

        return;
    }

    uint16_t move = this->_activeMoves[square->getSquare()];

    this->setPreviousSquares(this->_selectedSquare->getSquare(), square->getSquare());

    this->clearSelection();

    engine.makeMove(move);
}

bool Chess::isOwnPiece(Square *square, ColourType side) {
    Piece &piece = square->getPiece();

    return piece.getPiece() != PieceType::EMPTY && piece.getColour() == side;
}

} // namespace application::gui
