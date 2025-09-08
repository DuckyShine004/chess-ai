#pragma once

#include <cstdint>

namespace engine::board {

enum PieceType : uint8_t {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
    EMPTY = 6,
};

// clang-format off
inline constexpr const char* UNICODE_PIECES[2][12] = {
    {
        "♟",
        "♞",
        "♝",
        "♜",
        "♛",
        "♚"
    },
    {
        "♙",
        "♘",
        "♗",
        "♖",
        "♕",
        "♔"
    }
};

inline constexpr PieceType BACKRANK_PIECES[8] = {
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