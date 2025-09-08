#include "application/gui/Board.hpp"

#include "engine/board/Colour.hpp"

#include "utility/BoardUtility.hpp"

using namespace engine;

using namespace engine::board;

using namespace utility;

namespace application::gui {

Board::Board() {
    for (int square = 0; square < 64; ++square) {
        int rank = BoardUtility::getRank(square);
        int file = BoardUtility::getFile(square);

        ColourType colour;

        if (rank & 1) {
            if (file & 1) {
                colour = ColourType::BLACK;
            } else {
                colour = ColourType::WHITE;
            }
        } else {
            if (file & 1) {
                colour = ColourType::WHITE;
            } else {
                colour = ColourType::BLACK;
            }
        }

        this->_squares[square] = Square(rank, file, colour);
    }
}

Square &Board::getSquare(sf::Vector2i mousePosition) {
    return this->_squares[0];
}

void Board::update(Engine &engine) {
    for (int square = 0; square < 64; ++square) {
        Piece &piece = this->_squares[square].getPiece();

        piece.clear();

        for (uint8_t side = ColourType::WHITE; side <= ColourType::BLACK; ++side) {
            PieceType pieceType = engine.getPiece(square, static_cast<ColourType>(side));

            if (pieceType != PieceType::EMPTY) {
                piece.setPiece(pieceType);
                piece.setColour(static_cast<ColourType>(side));

                break;
            }
        }
    }
}

void Board::render(sf::RenderWindow &window) {
    for (Square &square : this->_squares) {
        square.render(window);
    }
}

} // namespace application::gui