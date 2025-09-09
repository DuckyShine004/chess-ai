#pragma once

#include <SFML/Graphics.hpp>

#include "engine/Engine.hpp"

#include "application/gui/Chess.hpp"

namespace application {

inline constexpr sf::Color BACKGROUND_COLOUR = sf::Color(22, 21, 18);

class Application {
  public:
    Application();

    void initialise();

    void run();

    void debug();

  private:
    engine::Engine _engine;

    sf::RenderWindow _window;

    application::gui::Chess _chess;

    void update();

    void render();
};

} // namespace application
