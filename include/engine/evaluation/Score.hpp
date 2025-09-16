#pragma once

namespace engine::evaluation::Score {

inline constexpr int CHECKMATE_SCORE = -32768;

[[nodiscard]] inline constexpr int getCheckMateScore(int ply);

[[nodiscard]] inline constexpr int getCheckMateScore(int ply) {
    return CHECKMATE_SCORE + ply;
}

} // namespace engine::evaluation::Score
