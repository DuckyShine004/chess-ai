#pragma once

#include "engine/board/Piece.hpp"

namespace engine::move {

struct Undo {
    uint8_t castleRights;

    int enPassantSquare;

    // Keep track of zobrist

    // Keep track of half moves

    engine::board::PieceType capturedPiece;

    Undo() : enPassantSquare(-1), capturedPiece(engine::board::PieceType::EMPTY) {
    }
};

} // namespace engine::move
