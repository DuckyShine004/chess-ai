#pragma once

#include <cstdint>

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

namespace engine::move {

struct Move {
    enum MoveType : uint8_t {
        NONE = 0,
    };

    int from; // from square index
    int to;

    // Check what type of move it is (mask variable)
    int8_t enPassantSquare;

    engine::board::PieceType piece;
    engine::board::ColourType colour;

    engine::board::PieceType capturedPiece;

    Move() {
        this->capturedPiece = engine::board::PieceType::EMPTY;

        this->enPassantSquare = -1;
    }

    Move(int from, int to, engine::board::PieceType piece, engine::board::ColourType colour) : from(from), to(to), piece(piece), colour(colour) {
        this->capturedPiece = engine::board::PieceType::EMPTY;

        this->enPassantSquare = -1;
    }
};

} // namespace engine::move