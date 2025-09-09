#include "application/gui/Square.hpp"

#include "utility/BoardUtility.hpp"

using namespace engine::board;

using namespace utility;

namespace application::gui {

Square::Square() : _isActive(false), _isAttacked(false) {
}

Square::Square(int rank, int file, ColourType colour) : _file(file), _rank(rank), _size(SIZE), _colour(colour), _isActive(false), _isHovered(false), _isAttacked(false) {
    this->_x = file * this->_size;
    this->_y = (7 - rank) * this->_size;

    this->_square = sf::RectangleShape(sf::Vector2f(SIZE, SIZE));

    this->_square.setFillColor(SQUARE_COLOURS[colour]);

    this->_square.setPosition(sf::Vector2f(this->_x, this->_y));
}

Piece &Square::getPiece() {
    return this->_piece;
}

int Square::getSquare() {
    return BoardUtility::getSquare(this->_rank, this->_file);
}

bool Square::isEmpty() {
    return this->_piece.isEmpty();
}

bool Square::isCollideWithPoint(sf::Vector2i position) {
    sf::FloatRect bounds = this->_square.getGlobalBounds();

    sf::Vector2f fPosition(position.x, position.y);

    return bounds.contains(fPosition);
}

void Square::setIsActive(bool isActive) {
    this->_isActive = isActive;
}

void Square::setIsHovered(bool isHovered) {
    this->_isHovered = isHovered;
}

void Square::setIsAttacked(bool isAttacked) {
    this->_isAttacked = isAttacked;
}

void Square::setIsPrevious(bool isPrevious) {
    if (isPrevious) {
        this->_square.setFillColor(PREVIOUS_COLOURS[this->_colour]);
    } else {
        this->_square.setFillColor(SQUARE_COLOURS[this->_colour]);
    }
}

bool Square::isAttacked() {
    return this->_isAttacked;
}

sf::Vector2f Square::getCentre() {
    float centreX = this->_x + (this->_size / 2.0f);
    float centreY = this->_y + (this->_size / 2.0f);

    return sf::Vector2f(centreX, centreY);
}

void Square::renderAttack(sf::RenderWindow &window) {
    float radius = SIZE / 8.0f;

    sf::Vector2f centre = this->getCentre();

    sf::CircleShape circle(radius);

    circle.setFillColor(ATTACK_COLOUR);

    circle.setPosition(sf::Vector2f(centre.x - radius, centre.y - radius));

    window.draw(circle);
}

void Square::renderBorder(sf::RenderWindow &window) {
    sf::RectangleShape square(sf::Vector2f(this->_size, this->_size));

    square.setPosition(sf::Vector2f(this->_x, this->_y));

    square.setFillColor(sf::Color::Transparent);

    square.setOutlineColor(BORDER_COLOUR);

    square.setOutlineThickness(BORDER_THICKNESS);

    window.draw(square);
}

void Square::render(sf::RenderWindow &window) {
    window.draw(this->_square);

    this->_piece.render(this->_x, this->_y, this->_size, window);

    if (this->_isAttacked) {
        this->renderAttack(window);
    }

    if (this->_isActive || this->_isHovered) {
        this->renderBorder(window);
    }
}

} // namespace application::gui
