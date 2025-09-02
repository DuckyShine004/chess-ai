#pragma once

#include "engine/board/Board.hpp"

namespace engine {

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