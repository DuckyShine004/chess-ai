#pragma once

#include "engine/board/Board.hpp"

using namespace engine::board;

namespace application {

class Application {
  public:
    Application();

    void run();

  private:
    Board _board;
};

} // namespace application