#pragma once

#include "engine/TempEngine.hpp"

namespace application {

class Application {
  public:
    Application();

    void run();

  private:
    engine::TempEngine _engine;
};

} // namespace application