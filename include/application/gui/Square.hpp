#pragma once

#include <SFML/Graphics.hpp>

#include <engine/board/Colour.hpp>

namespace application::gui {

inline constexpr float SIZE = 100.0f;

inline constexpr sf::Color COLOURS[2] = {
    sf::Color(240, 217, 181),
    sf::Color(181, 136, 99),
};

class Square {
  public:
    Square();

    Square(int rank, int file, engine::board::Colour colour);

    void render(sf::RenderWindow &window);

  private:
    sf::RectangleShape _square;

    engine::board::Colour _colour;

    float _file;
    float _rank;
    float _size;
};

} // namespace application::gui