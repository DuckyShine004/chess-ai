#pragma once

#include <cstdint>

#include "engine/board/Piece.hpp"

// Encoding:
// from and to squares, 0-5,6-11
// 12-(12+MoveType)
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

inline constexpr uint16_t FROM_MASK = 0b0000000000111111;

inline constexpr uint16_t TO_MASK = 0b0000111111000000;

inline constexpr uint16_t MOVE_TYPE_MASK = 0b1111000000000000;

inline constexpr int TO_OFFSET = 6;

inline constexpr int MOVE_TYPE_OFFSET = 12;

// TODO: encode move struct to optimise memory
inline constexpr void setFrom(uint16_t &move, int square);

inline constexpr void setTo(uint16_t &move, int square);

inline constexpr void setMoveType(uint16_t &move, MoveType moveType);

[[nodiscard]] inline constexpr uint16_t getMove(int from, int to, MoveType moveType);

[[nodiscard]] inline constexpr int getFrom(uint16_t move);

[[nodiscard]] inline constexpr int getTo(uint16_t move);

[[nodiscard]] inline constexpr bool isQuiet(uint16_t move);

inline constexpr void setFromSquare(uint16_t &move, int square) {
    move |= square;
}

inline constexpr void setToSquare(uint16_t &move, int square) {
    move |= (square << TO_OFFSET);
}

inline constexpr void setMoveType(uint16_t &move, MoveType moveType) {
    move |= (moveType << MOVE_TYPE_OFFSET);
}

[[nodiscard]] inline constexpr uint16_t getMove(int from, int to, MoveType moveType) {
    return from | (to << TO_OFFSET) | (moveType << MOVE_TYPE_OFFSET);
}

[[nodiscard]] inline constexpr int getFrom(uint16_t move) {
    return move & FROM_MASK;
}

[[nodiscard]] inline constexpr int getTo(uint16_t move) {
    return (move & TO_MASK) >> TO_OFFSET;
}

[[nodiscard]] inline constexpr bool isQuiet(uint16_t move) {
    return (move >> MOVE_TYPE_OFFSET) == MoveType::QUIET;
}

[[nodiscard]] inline constexpr bool isCapture(uint16_t move) {
    return (move >> MOVE_TYPE_OFFSET) == MoveType::CAPTURE;
}

[[nodiscard]] inline constexpr bool isDoublePawn(uint16_t move) {
    return (move >> MOVE_TYPE_OFFSET) == MoveType::DOUBLE_PAWN;
}

[[nodiscard]] inline constexpr bool isQueenCastle(uint16_t move) {
    return (move >> MOVE_TYPE_OFFSET) == MoveType::QUEEN_CASTLE;
}

[[nodiscard]] inline constexpr bool isKingCastle(uint16_t move) {
    return (move >> MOVE_TYPE_OFFSET) == MoveType::KING_CASTLE;
}

// [[nodiscard]] inline constexpr bool isPromotionQuiet(uint16_t move) {
//     MoveType moveType (move >> MOVE_TYPE_OFFSET) == MoveType::KING_CASTLE;
// }

struct Move {
    int from;
    int to;

    MoveType moveType;

    Move() : moveType(MoveType::QUIET) {
    }

    Move(int from, int to, MoveType moveType) : from(from), to(to), moveType(moveType) {
    }

    bool isQuiet() {
        return moveType == MoveType::QUIET;
    }

    bool isCapture() {
        return moveType == MoveType::CAPTURE;
    }

    bool isDoublePawn() {
        return moveType == MoveType::DOUBLE_PAWN;
    }

    bool isEnPassant() {
        return moveType == MoveType::EN_PASSANT;
    }

    bool isQueenCastle() {
        return moveType == MoveType::QUEEN_CASTLE;
    }

    bool isKingCastle() {
        return moveType == MoveType::KING_CASTLE;
    }

    bool isPromotionQuiet() {
        return moveType >= MoveType::KNIGHT_PROMOTION && moveType <= MoveType::QUEEN_PROMOTION;
    }

    bool isPromotionCapture() {
        return moveType >= MoveType::KNIGHT_PROMOTION_CAPTURE && moveType <= MoveType::QUEEN_PROMOTION_CAPTURE;
    }

    engine::board::PieceType getPromotionPiece() const {
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
    uint16_t moves[256];

    int size;

    MoveList() : size(0) {
    }

    void add(int from, int to, MoveType moveType) {
        moves[size++] = getMove(from, to, moveType);
    }

    void add(const uint16_t &move) {
        moves[size++] = move;
    }
};

} // namespace engine::move
