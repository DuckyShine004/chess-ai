#pragma once

#include <SFML/Graphics.hpp>

#include "engine/Engine.hpp"

#include "application/gui/Chess.hpp"

namespace application {

class Application {
  public:
    Application();

    void initialise();

    void run();

  private:
    engine::Engine _engine;

    sf::RenderWindow _window;

    application::gui::Chess _chess;

    void update();

    void render();
};

} // namespace application
