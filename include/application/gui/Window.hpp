#pragma once

#include <cstdint>

#include <SFML/Graphics.hpp>

namespace application::gui {

class Window {
  public:
    Window();

    int getWidth();

    int getHeight();

  private:
    static inline constexpr int _WIDTH = 1440;
    static inline constexpr int _HEIGHT = 900;

    int _width;
    int _height;

    sf::VideoMode _mode;

    sf::RenderWindow _window;
};

} // namespace application::gui