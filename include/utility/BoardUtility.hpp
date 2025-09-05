#pragma once

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

namespace utility::BoardUtility {

[[nodiscard]] inline constexpr int getSquare(int rank, int file) {
    return (rank << 3) | file;
}

[[nodiscard]] inline constexpr int getRank(int square) {
    return square >> 3;
}

[[nodiscard]] inline constexpr int getFile(int square) {
    return square & 7;
}

[[nodiscard]] inline constexpr int getPieceIndex(engine::board::Piece piece, engine::board::Colour colour) {
    return (piece - 1) + 6 * colour;
}

} // namespace utility::BoardUtility