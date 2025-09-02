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

// clang-format off
inline constexpr const char* UNICODE_PIECES[12] = {
    "♙",
    "♘",
    "♗",
    "♖",
    "♕",
    "♔",
    "♟",
    "♞",
    "♝",
    "♜",
    "♛",
    "♚",
};

inline constexpr Piece BACKRANK_PIECES[8] = {
    ROOK,
    KNIGHT,
    BISHOP,
    KING,
    QUEEN,
    BISHOP,
    KNIGHT,
    ROOK,
};
// clang-format on

} // namespace engine::board