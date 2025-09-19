#pragma once

namespace engine::evaluation::Score {

inline constexpr int INF = 50000;

// inline constexpr int CHECKMATE_SCORE = -32768;
inline constexpr int CHECKMATE_SCORE = 49000;
inline constexpr int CHECKMATE_OFFSET = 1000;
inline constexpr int CHECKMATE_THRESHOLD = CHECKMATE_SCORE - CHECKMATE_OFFSET;

[[nodiscard]] inline constexpr int getCheckMateScore(int ply);

[[nodiscard]] inline constexpr int getCheckMateScore(int ply) {
    return -CHECKMATE_SCORE + ply;
}

} // namespace engine::evaluation::Score
