#pragma once

#include <cstdint>
#include <utility>

#include "engine/board/Piece.hpp"

namespace engine::move {

enum MoveType : uint8_t {
    QUIET = 0,
    CAPTURE = 1,
    DOUBLE_PAWN = 2,
    EN_PASSANT = 3,
    KING_CASTLE = 4,
    QUEEN_CASTLE = 5,
    KNIGHT_PROMOTION = 6,
    BISHOP_PROMOTION = 7,
    ROOK_PROMOTION = 8,
    QUEEN_PROMOTION = 9,
    KNIGHT_PROMOTION_CAPTURE = 10,
    BISHOP_PROMOTION_CAPTURE = 11,
    ROOK_PROMOTION_CAPTURE = 12,
    QUEEN_PROMOTION_CAPTURE = 13,
};

struct Move {
    int from;
    int to;

    MoveType moveType;

    Move() : moveType(MoveType::QUIET) {
    }

    Move(int from, int to, MoveType moveType) : from(from), to(to), moveType(moveType) {
    }

    bool isPromotion() {
        return moveType >= MoveType::KNIGHT_PROMOTION && moveType <= MoveType::QUEEN_PROMOTION_CAPTURE;
    }

    bool isPromotionQuiet() {
        return moveType >= MoveType::KNIGHT_PROMOTION && moveType <= MoveType::QUEEN_PROMOTION;
    }

    bool isPromotionCapture() {
        return moveType >= MoveType::KNIGHT_PROMOTION_CAPTURE && moveType <= MoveType::QUEEN_PROMOTION_CAPTURE;
    }

    engine::board::PieceType getPromotionPiece() {
        if (moveType == MoveType::KNIGHT_PROMOTION || moveType == MoveType::KNIGHT_PROMOTION_CAPTURE) {
            return engine::board::PieceType::KNIGHT;
        }

        if (moveType == MoveType::BISHOP_PROMOTION || moveType == MoveType::BISHOP_PROMOTION_CAPTURE) {
            return engine::board::PieceType::BISHOP;
        }

        if (moveType == MoveType::ROOK_PROMOTION || moveType == MoveType::ROOK_PROMOTION_CAPTURE) {
            return engine::board::PieceType::ROOK;
        }

        return engine::board::PieceType::QUEEN;
    }
};

struct MoveList {
    Move moves[64];

    int size;

    MoveList() : size(0) {
    }

    void add(int from, int to, MoveType moveType) {
        moves[size++] = Move(from, to, moveType);
    }

    void add(const Move &move) {
        moves[size++] = move;
    }
};

} // namespace engine::move
