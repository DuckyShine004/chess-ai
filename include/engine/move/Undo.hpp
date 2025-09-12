#pragma once

#include "engine/board/Piece.hpp"

namespace engine::move {

struct Undo {
    uint8_t castleRights;

    int8_t enPassantSquare;

    // Keep track of zobrist

    uint16_t halfMove;

    engine::board::PieceType capturedPiece;

    Undo() : enPassantSquare(-1), capturedPiece(engine::board::PieceType::EMPTY), halfMove(0) {
    }
};

} // namespace engine::move
