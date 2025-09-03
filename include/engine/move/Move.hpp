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
// clang-format on

} // namespace engine::move