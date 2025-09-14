#include <thread>

#include <SFML/Window.hpp>

#include "application/gui/Chess.hpp"

#include "engine/move/Move.hpp"

#include "engine/board/Colour.hpp"

using namespace engine;

using namespace engine::board;

using namespace engine::move;

namespace application::gui {

Chess::Chess() : _isClicking(false), _isWaitingForEngine(false), _selectedSquare(nullptr), _previousFrom(-1), _previousTo(-1) {
}

void Chess::move(sf::RenderWindow &window, Engine &engine, sf::Vector2i mousePosition) {
    ColourType side = engine.getSide();

    if (side != ColourType::WHITE) {
        return;
    }

    if (this->_promotion.isPromoting()) {
        this->_promotion.makePromotionMove(engine, mousePosition);

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

void Chess::makeEngineMove(Engine &engine) {
}

void Chess::update(sf::RenderWindow &window, Engine &engine) {
    this->_board.update(window, engine);

    if (engine.getSide() != ColourType::WHITE) {
        std::thread makeEngineMoveTask(this->makeEngineMove, engine);

        this->_isWaitingForEngine.store(true);

        while (!this->_isWaitingForEngine.load()) {
            std::this_thread::yield();
            uint16_t &move = engine.getMove(); // TODO: thread

            int from = Move::getFrom(move);
            int to = Move::getTo(move);

            this->setPreviousSquares(from, to);

            engine.makeMove(move);

            this->_isWaitingForEngine.store(false);
        }
    }
}

void Chess::render(sf::RenderWindow &window) {
    this->_board.render(window);

    this->_promotion.render(window);
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

    if (Move::isGeneralPromotion(move)) {
        this->_promotion.setIsPromoting(true);
        this->_promotion.setMove(move);
    } else {
        engine.makeMove(move);
    }

    this->setPreviousSquares(this->_selectedSquare->getSquare(), square->getSquare());

    this->clearSelection();
}

bool Chess::isOwnPiece(Square *square, ColourType side) {
    Piece &piece = square->getPiece();

    return piece.getPiece() != PieceType::EMPTY && piece.getColour() == side;
}

} // namespace application::gui
