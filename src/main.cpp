#include <iostream>
#include <limits>

// // USING NEGAMAX- requires symmetric evaluation in relation to the side to moveA
// int getMaterialScore() {
//     return 0;
// }

// int getMobilityScore() {
//     return 0;
// }

// int evaluate() {
//     int materialScore = getMaterialScore();
//     int mobilityScore = getMobilityScore();

//     int score = materialScore + mobilityScore;
//     int sideToMove = 1; // White = +1, Black=-1

//     return score * sideToMove;
// }

// int quiescence(int alpha, int beta) {
//     int evaluation = evaluate();

//     // Standing pat- allows quiescence to stabilise
//     int bestScore = evaluate();

//     if (bestScore >= beta) {
//         return bestScore;
//     }

//     if (bestScore > alpha) {
//         alpha = bestScore;
//     }

//     // Examine all captures: TODO
//     // Make capture: TODO
//     int score = -quiescence(-beta, -alpha);
//     // Take back capture: TODO
//     if (score >= beta) {
//         return score;
//     }

//     if (score > bestScore) {
//         bestScore = score;
//     }

//     if (score > alpha) {
//         alpha = score;
//     }

//     return score;
// }

// int search(int alpha, int beta, int depth) {
//     // Perform quiescence search after
//     if (depth == 0) {
//         return quiescence(alpha, beta);
//     }

//     int bestScore = std::numeric_limits<int>::min();

//     // For all moves: TODO
//     int score = -search(-beta, -alpha, depth - 1);

//     if (score > bestScore) {
//         bestScore = score;

//         if (score > alpha) {
//             alpha = score;
//         }

//         if (score >= beta) {
//             return bestScore;
//         }
//     }

//     return bestScore;
// }

#include "application/Application.hpp"

using namespace application;

int main() {
    std::cout << "Hello world!" << std::endl;

    Application application;

    return 0;
}