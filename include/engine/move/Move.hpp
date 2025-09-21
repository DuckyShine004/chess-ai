#pragma once

#include <string>
#include <cstdint>

#include "engine/board/Piece.hpp"

#include "logger/LoggerMacros.hpp"
#include "utility/BoardUtility.hpp"

// Encoding:
// from and to squares, 0-5,6-11
// move type: 12-16
namespace engine::move::Move {

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

inline constexpr int PROMOTION_QUIET_OFFSET = 5;

inline constexpr int PROMOTION_CAPTURE_OFFSET = 9;

[[nodiscard]] inline constexpr uint16_t getMove(int from, int to, MoveType moveType);

[[nodiscard]] inline constexpr uint16_t getPromotionMove(int from, int to, engine::board::PieceType piece, bool isCapture);

[[nodiscard]] inline constexpr int getFrom(uint16_t move);

[[nodiscard]] inline constexpr int getTo(uint16_t move);

[[nodiscard]] inline constexpr bool isQuiet(uint16_t move);

[[nodiscard]] inline constexpr bool isCapture(uint16_t move);

[[nodiscard]] inline constexpr bool isDoublePawn(uint16_t move);

[[nodiscard]] inline constexpr bool isEnPassant(uint16_t move);

[[nodiscard]] inline constexpr bool isQueenCastle(uint16_t move);

[[nodiscard]] inline constexpr bool isKingCastle(uint16_t move);

[[nodiscard]] inline constexpr bool isPromotionQuiet(uint16_t move);

[[nodiscard]] inline constexpr bool isPromotionCapture(uint16_t move);

[[nodiscard]] inline constexpr engine::board::PieceType getPromotionPiece(uint16_t move);

[[nodiscard]] inline constexpr bool isKiller(uint16_t move);

[[nodiscard]] inline constexpr bool isHistory(uint16_t move);

[[nodiscard]] inline constexpr bool isGeneralCapture(uint16_t move);

[[nodiscard]] inline constexpr bool isTactical(uint16_t move);

[[nodiscard]] inline constexpr bool isLMR(uint16_t move);

[[nodiscard]] inline std::string getFromToString(uint16_t move);

[[nodiscard]] inline constexpr uint16_t getMove(int from, int to, MoveType moveType) {
    return from | (to << TO_OFFSET) | (moveType << MOVE_TYPE_OFFSET);
}

[[nodiscard]] inline constexpr uint16_t getPromotionMove(int from, int to, engine::board::PieceType piece, bool isCapture) {
    MoveType moveType = (isCapture) ? static_cast<MoveType>(piece + PROMOTION_CAPTURE_OFFSET) : static_cast<MoveType>(piece + PROMOTION_QUIET_OFFSET);

    return getMove(from, to, moveType);
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

[[nodiscard]] inline constexpr bool isEnPassant(uint16_t move) {
    return (move >> MOVE_TYPE_OFFSET) == MoveType::EN_PASSANT;
}

[[nodiscard]] inline constexpr bool isGeneralCastle(uint16_t move) {
    MoveType moveType = static_cast<MoveType>(move >> MOVE_TYPE_OFFSET);

    return moveType == MoveType::KING_CASTLE || moveType == MoveType::QUEEN_CASTLE;
}

[[nodiscard]] inline constexpr bool isQueenCastle(uint16_t move) {
    return (move >> MOVE_TYPE_OFFSET) == MoveType::QUEEN_CASTLE;
}

[[nodiscard]] inline constexpr bool isKingCastle(uint16_t move) {
    return (move >> MOVE_TYPE_OFFSET) == MoveType::KING_CASTLE;
}

[[nodiscard]] inline constexpr bool isPromotionQuiet(uint16_t move) {
    MoveType moveType = static_cast<MoveType>(move >> MOVE_TYPE_OFFSET);

    return MoveType::KNIGHT_PROMOTION <= moveType && moveType <= MoveType::QUEEN_PROMOTION;
}

[[nodiscard]] inline constexpr bool isPromotionCapture(uint16_t move) {
    MoveType moveType = static_cast<MoveType>(move >> MOVE_TYPE_OFFSET);

    return MoveType::KNIGHT_PROMOTION_CAPTURE <= moveType && moveType <= MoveType::QUEEN_PROMOTION_CAPTURE;
}

[[nodiscard]] inline constexpr engine::board::PieceType getPromotionPiece(uint16_t move) {
    MoveType moveType = static_cast<MoveType>(move >> MOVE_TYPE_OFFSET);

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

[[nodiscard]] inline constexpr bool isKiller(uint16_t move) {
    MoveType moveType = static_cast<MoveType>(move >> MOVE_TYPE_OFFSET);

    return moveType == MoveType::QUIET || moveType == MoveType::DOUBLE_PAWN || moveType == MoveType::KING_CASTLE || moveType == MoveType::QUEEN_CASTLE || (MoveType::KNIGHT_PROMOTION <= moveType && moveType <= MoveType::QUEEN_PROMOTION);
}

[[nodiscard]] inline constexpr bool isHistory(uint16_t move) {
    MoveType moveType = static_cast<MoveType>(move >> MOVE_TYPE_OFFSET);

    return moveType == MoveType::QUIET || moveType == MoveType::DOUBLE_PAWN || moveType == MoveType::KING_CASTLE || moveType == MoveType::QUEEN_CASTLE;
}

[[nodiscard]] inline constexpr bool isGeneralCapture(uint16_t move) {
    MoveType moveType = static_cast<MoveType>(move >> MOVE_TYPE_OFFSET);

    return moveType == MoveType::CAPTURE || moveType == MoveType::EN_PASSANT || (MoveType::KNIGHT_PROMOTION_CAPTURE <= moveType && moveType <= MoveType::QUEEN_PROMOTION_CAPTURE);
}

[[nodiscard]] inline constexpr bool isGeneralPromotion(uint16_t move) {
    MoveType moveType = static_cast<MoveType>(move >> MOVE_TYPE_OFFSET);

    return MoveType::KNIGHT_PROMOTION <= moveType && moveType <= MoveType::QUEEN_PROMOTION_CAPTURE;
}

[[nodiscard]] inline constexpr bool isTactical(uint16_t move) {
    MoveType moveType = static_cast<MoveType>(move >> MOVE_TYPE_OFFSET);

    return moveType == MoveType::CAPTURE || moveType == MoveType::EN_PASSANT || (MoveType::KNIGHT_PROMOTION <= moveType && moveType <= MoveType::QUEEN_PROMOTION_CAPTURE);
}

[[nodiscard]] inline constexpr bool isLMR(uint16_t move) {
    MoveType moveType = static_cast<MoveType>(move >> MOVE_TYPE_OFFSET);

    return !(moveType == MoveType::CAPTURE || moveType == MoveType::EN_PASSANT || moveType == MoveType::KING_CASTLE || moveType == MoveType::QUEEN_CASTLE || (MoveType::KNIGHT_PROMOTION <= moveType && moveType <= MoveType::QUEEN_PROMOTION_CAPTURE));
}

[[nodiscard]] inline std::string getFromToString(uint16_t move) {
    int from = Move::getFrom(move);
    int to = Move::getTo(move);

    return utility::BoardUtility::getPositionFromSquare(from) + utility::BoardUtility::getPositionFromSquare(to);
}

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

} // namespace engine::move::Move
