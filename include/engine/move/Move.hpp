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

    Move() {
        enPassantSquare = -1;
    }
};

} // namespace engine::move