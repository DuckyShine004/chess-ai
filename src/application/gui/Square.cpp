#include "application/gui/Square.hpp"

using namespace engine::board;

namespace application::gui {

Square::Square() = default;

Square::Square(int rank, int file, ColourType colour) : _file(file), _rank(rank), _size(SIZE), _colour(colour) {
    this->_x = file * this->_size;
    this->_y = (7 - rank) * this->_size;

    this->_square = sf::RectangleShape(sf::Vector2f(SIZE, SIZE));

    this->_square.setFillColor(COLOURS[colour]);

    this->_square.setPosition(sf::Vector2f(this->_x, this->_y));
}

Piece &Square::getPiece() {
    return this->_piece;
}

bool Square::isEmpty() {
    return this->_piece.isEmpty();
}

void Square::render(sf::RenderWindow &window) {
    window.draw(this->_square);

    this->_piece.render(this->_x, this->_y, this->_size, window);
}

} // namespace application::gui