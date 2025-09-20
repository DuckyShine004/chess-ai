#pragma once

#include <cstdint>

namespace engine::evaluation::pesto {

enum GamePhase : uint8_t {
    OPENING = 0,
    ENDGAME = 1,
    MIDDLEGAME = 2,
};

inline constexpr int OPENING_SCORE = 6192;
inline constexpr int ENDGAME_SCORE = 518;

inline constexpr int GAME_PHASE_VALUES[6] = { 0, 1, 1, 2, 4, 0 };

} // namespace engine::evaluation::pesto
