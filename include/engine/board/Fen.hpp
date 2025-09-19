#pragma once

namespace engine::board {

inline constexpr const char *INITIAL_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// https://www.chessprogramming.org/Perft_Results#Position_5
// clang-format off
inline constexpr const char *POSITIONS[5] = {
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - - -",
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
};

// Two bishops [-]
// Two knights [+]
// One rook [+]
// Two rooks [+]
// One queen [+]
inline constexpr const char *PUZZLE_POSITIONS[] = {
    "2b1kb2/8/8/8/8/8/8/4K3 w - - 0 1",
    "1n2k1n1/8/8/8/8/8/8/4K3 w - - 0 1",
    "4k2r/8/8/8/8/8/8/4K3 w - - 0 1",
    "4k3/8/3K4/8/7r/1r6/8/8 w - - 0 1",
    "3qk3/8/8/8/8/8/8/4K3 w - - 0 1",
};

// Tricky king and pawn endgame [+]
// Tricky mate in one [-]
// Tricky endgame with possible checkmate [-]
inline constexpr const char* NMP_POSITIONS[] = {
    "8/8/8/8/8/3k4/2p5/2K5 w - - 0 1",
    "6k1/pp6/2p5/3p4/3Pp1p1/2P1PpPq/PPQ2P2/3R2K1 w - - 0 1",
    "8/6pp/p2p4/2k1p3/4n3/1r5N/7K/8 b - - 0 1",
};
// clang-format on

inline constexpr const char *KILLER_POSITION = "6k1/3q1pp1/pp3n1p/1r6/5P2/1P4P1/PQ4BP/2R3K1 w KQkq - 0 1";

inline constexpr const char *PROMOTION_POSITIONS[] = {
    "3k4/ppp2pp1/2b4p/8/8/8/4K3/8 b - - 0 1",
};

inline constexpr const char *TEST_POSITIONS[1] = {
    "r1b1k2r/pppp1ppp/2n1pq2/2Q5/3PPn2/5N2/PPP2PPP/2KR1B1R b Kkq - 0 1",
};

inline constexpr const char *EN_PASSANT_POSITIONS[] = {
    "7k/K7/8/8/5p2/8/4P3/8 w - - 0 1",
};
// inline constexpr const char *EN_PASSANT_POSITIONS[1] = {
//     "4n3/5P2/8/1k6/8/8/8/2K5 w - - 0 1",
// };
//
} // namespace engine::board
