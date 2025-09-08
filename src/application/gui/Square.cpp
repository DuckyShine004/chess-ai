#include "application/gui/Square.hpp"

using namespace engine::board;

namespace application::gui {

Square::Square() = default;

Square::Square(int rank, int file, ColourType colour) : _file(file), _rank(rank), _size(SIZE), _colour(colour) {
    float x = file * this->_size;
    float y = (7 - rank) * this->_size;

    this->_square = sf::RectangleShape(sf::Vector2f(SIZE, SIZE));

    this->_square.setFillColor(COLOURS[colour]);

    this->_square.setPosition(sf::Vector2f(x, y));
}

void Square::render(sf::RenderWindow &window) {
    window.draw(this->_square);
}

} // namespace application::gui