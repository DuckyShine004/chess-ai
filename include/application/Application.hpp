#pragma once

#include "engine/board/Board.hpp"

using namespace engine::board;

namespace application {

class Application {
  public:
    Application();

  private:
    Board _board;
};

} // namespace application