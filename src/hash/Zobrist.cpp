#include "engine/hash/Zobrist.hpp"

#include "engine/board/Board.hpp"

#include "utility/BoardUtility.hpp"
#include "utility/RandomUtility.hpp"

using namespace engine::board;

using namespace utility;

namespace engine::hash {

Zobrist::Zobrist(uint64_t seed) {
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 12; ++j) {
            this->_key.piece[i][j] = RandomUtility::getRandomU64();
        }
    }

    this->_key.side = RandomUtility::getRandomU64();
};

Key &Zobrist::getKey() {
    return this->_key;
}

uint64_t Zobrist::hash(Board &board) {
    uint64_t hash = 0;

    if (!board.isWhiteTurn()) {
        hash ^= this->_key.side;
    }

    for (int i = 0; i < 64; ++i) {
        Piece piece = board.getPieceFromSquare(i);

        if (piece != Piece::EMPTY) {
            Colour colour = board.getColourFromSquare(i);

            int j = BoardUtility::getPieceIndex(piece, colour);

            hash ^= this->_key.piece[i][j];
        }
    }

    return hash;
}

} // namespace engine::hash