#pragma once

#include <string>
#include <iostream>

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

namespace utility::BoardUtility {

[[nodiscard]] inline constexpr int getSquare(int rank, int file);

[[nodiscard]] inline constexpr int getRank(int square);

[[nodiscard]] inline constexpr int getFile(int square);

[[nodiscard]] inline constexpr int getPieceIndex(engine::board::PieceType piece, engine::board::ColourType colour);

[[nodiscard]] inline int getSquareFromPosition(std::string &position);

[[nodiscard]] inline std::string getPositionFromSquare(int square);

[[nodiscard]] inline int getMirroredSquare(int square);

[[nodiscard]] inline constexpr engine::board::PieceType getPiece(uint64_t board[2][6], int rank, int file, engine::board::ColourType side);

[[nodiscard]] inline constexpr engine::board::PieceType getPiece(uint64_t board[2][6], int square, engine::board::ColourType side);

[[nodiscard]] inline constexpr engine::board::ColourType getOtherSide(engine::board::ColourType side);

inline void printBoard(uint64_t board[2][6]);

[[nodiscard]] inline constexpr int getSquare(int rank, int file) {
    return (rank << 3) | file;
}

[[nodiscard]] inline constexpr int getRank(int square) {
    return square >> 3;
}

[[nodiscard]] inline constexpr int getFile(int square) {
    return square & 7;
}

[[nodiscard]] inline constexpr int getPieceIndex(engine::board::PieceType piece, engine::board::ColourType colour) {
    return (piece - 1) + 6 * colour;
}

[[nodiscard]] inline int getSquareFromPosition(std::string &position) {
    int file = position[0] - 'a';
    int rank = position[1] - '1';

    return getSquare(rank, file);
}

[[nodiscard]] inline std::string getPositionFromSquare(int square) {
    char position[2];

    position[0] = getFile(square) + 'a';
    position[1] = getRank(square) + '1';

    return position;
}

[[nodiscard]] inline int getMirroredSquare(int square) {
    int rank = getRank(square);
    int file = getFile(square);

    return getSquare(7 - rank, file);
}

[[nodiscard]] inline constexpr engine::board::PieceType getPiece(uint64_t board[2][6], int rank, int file, engine::board::ColourType side) {
    return getPiece(board, getSquare(rank, file), side);
}

[[nodiscard]] inline constexpr engine::board::PieceType getPiece(uint64_t board[2][6], int square, engine::board::ColourType side) {
    for (uint8_t piece = 0; piece < 6; ++piece) {
        if (board[side][piece] & (1ULL << square)) {
            return static_cast<engine::board::PieceType>(piece);
        }
    }

    return engine::board::PieceType::EMPTY;
}

[[nodiscard]] inline constexpr engine::board::ColourType getOtherSide(engine::board::ColourType side) {
    return static_cast<engine::board::ColourType>(side ^ 1);
}

inline void printBoard(uint64_t board[2][6]) {
    std::cout << "  +---+---+---+---+---+---+---+---+\n";

    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << " |";

        for (int file = 0; file < 8; ++file) {
            engine::board::PieceType piece = getPiece(board, rank, file, engine::board::ColourType::WHITE);

            if (piece != engine::board::PieceType::EMPTY) {
                std::cout << ' ' << engine::board::UNICODE_PIECES[0][piece] << " |";

                continue;
            }

            piece = getPiece(board, rank, file, engine::board::ColourType::BLACK);

            if (piece == engine::board::PieceType::EMPTY) {
                std::cout << "   |";
            } else {
                std::cout << ' ' << engine::board::UNICODE_PIECES[1][piece] << " |";
            }
        }

        std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    }

    std::cout << "    A   B   C   D   E   F   G   H\n\n";
}

} // namespace utility::BoardUtility