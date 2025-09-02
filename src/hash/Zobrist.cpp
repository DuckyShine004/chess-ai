#include "engine/hash/Zobrist.hpp"

#include "engine/board/Board.hpp"

using namespace engine::board;

namespace hash {

Zobrist::Zobrist(uint64_t seed) {
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 12; ++j) {
            this->_key.piece[i][j] = 0;
        }
    }

    this->_key.side = 0;
};

uint64_t Zobrist::hash(Board &board) {
    uint64_t hash = 0;

    if (!board.isWhiteTurn()) {
        hash ^= this->_key.side;
    }

    for (int i = 0; i < 64; ++i) {
        Piece piece = board.getPieceFromSquare(i);

        if (piece != Piece::EMPTY) {
            Colour colour = board.getColourFromSquare(i);

            int j = (piece - 1) + 6 * colour;

            hash ^= this->_key.piece[i][j];
        }
    }

    return hash;
}

} // namespace hash