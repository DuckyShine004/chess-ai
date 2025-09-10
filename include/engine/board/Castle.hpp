#pragma once

#include <cstdint>

#include "engine/board/Square.hpp"

namespace engine::board {

enum Castle : uint8_t {
    WHITE_KING = 0,
    WHITE_QUEEN = 1,
    BLACK_KING = 2,
    BLACK_QUEEN = 3,
};

inline constexpr uint64_t CASTLE_EMPTY_MASK[4] = {
    0x0000000000000060,
    0x000000000000000e,
    0x6000000000000000,
    0x0e00000000000000,
};

inline constexpr uint64_t CASTLE_SAFE_MASK[4] = {
    0x0000000000000070,
    0x000000000000001c,
    0x7000000000000000,
    0x1c00000000000000,
};

inline constexpr Square KING_ORIGIN_SQUARES[2] = {
    Square::e1,
    Square::e8,
};

inline constexpr Square ROOK_ORIGIN_SQUARES[4] = {
    Square::h1,
    Square::a1,
    Square::h8,
    Square::a8,
};

inline constexpr Square KING_TO_SQUARES[4] = {
    Square::g1,
    Square::c1,
    Square::g8,
    Square::c8,
};

inline constexpr Square ROOK_TO_SQUARES[4] = {
    Square::f1,
    Square::d1,
    Square::f8,
    Square::d8,
};

} // namespace engine::board
