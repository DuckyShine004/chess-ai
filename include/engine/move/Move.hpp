#pragma once

#include <cstdint>

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

namespace engine::move {

struct Move {
    int from; // from square index
    int to;

    // Check what type of move it is (mask variable)
    int8_t enPassantSquare;

    engine::board::Piece piece;
    engine::board::Colour colour;

    engine::board::Piece capturedPiece;

    Move() {
        this->capturedPiece = engine::board::Piece::EMPTY;

        this->enPassantSquare = -1;
    }

    Move(int from, int to, engine::board::Piece piece, engine::board::Colour colour) {
        this->from = from;
        this->to = to;

        this->piece = piece;
        this->colour = colour;

        this->capturedPiece = engine::board::Piece::EMPTY;

        this->enPassantSquare = -1;
    }
};

// clang-format off
inline constexpr int SECOND_RANK[2] = {
    1,
    6
};

inline constexpr int PAWN_MOVES[2][2] = {
    {1, 2},
    {-1, -2}
};

inline constexpr int PAWN_ATTACKS[2][2][2] = {
    {
        {1, -1},
        {1, 1}
    },
    {
        {-1, -1},
        {-1, 1}
    }
};

inline constexpr int KNIGHT_MOVES[8][2] = {
    {-2, -1},
    {-2, 1},
    {-1, -2},
    {-1, 2},
    {1, -2},
    {1, 2},
    {2, -1},
    {2, 1}
};

inline constexpr int BISHOP_MOVES[4][2] = {
    {-1, -1},
    {-1, 1},
    {1, -1},
    {1, 1}
};

inline constexpr int ROOK_MOVES[4][2] = {
    {1, 0},
    {-1, 0},
    {0, -1},
    {0, 1}
};
// clang-format on

} // namespace engine::move