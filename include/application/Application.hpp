#pragma once

#include "engine/Engine.hpp"

namespace application {

class Application {
  public:
    Application();

    void run();

  private:
    engine::Engine _engine;
};

} // namespace application