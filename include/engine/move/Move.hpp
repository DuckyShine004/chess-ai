#pragma once

#include <cstdint>

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

namespace engine::move {

enum MoveType : uint8_t {
    QUIET = 0,
    CAPTURE = 1,
    EN_PASSANT = 2,
    KING_CASTLE = 3,
    QUEEN_CASTLE = 4,
    NONE = 5,
};

struct Move {
    int from;
    int to;

    engine::board::PieceType piece;
    engine::board::ColourType colour;

    engine::board::PieceType capturedPiece;

    MoveType moveType;

    Move() {
        this->initialise();
    }

    Move(int from, int to, engine::board::PieceType piece, engine::board::ColourType colour) : from(from), to(to), piece(piece), colour(colour) {
        this->initialise();
    }

    void initialise() {
        this->capturedPiece = engine::board::PieceType::EMPTY;

        this->moveType = MoveType::NONE;
    }
};

} // namespace engine::move
