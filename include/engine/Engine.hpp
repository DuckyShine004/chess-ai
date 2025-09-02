#pragma once

#include "engine/board/Board.hpp"

namespace engine {
// // USING NEGAMAX- requires symmetric evaluation in relation to the side to moveA
// int getMaterialScore() {
//     return 0;
// }

// int getMobilityScore() {
//     return 0;
// }

class Engine {
  public:
    Engine();

    engine::board::Board &getBoard();

    int search(int alpha, int beta, int depth);

  private:
    engine::board::Board _board;

    int quiescence(int alpha, int beta);

    int evaluate();

    int getMaterialScore();

    int getMobilityScore();
};

} // namespace engine