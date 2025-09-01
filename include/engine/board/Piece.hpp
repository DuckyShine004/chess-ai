#pragma once

#include <cstdint>

namespace engine::board {

enum Piece : uint8_t {
    EMPTY = 0,
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6,
};

} // namespace engine::board