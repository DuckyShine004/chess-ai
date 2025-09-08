#include <string>
#include <stdexcept>

#include <SFML/Graphics/Texture.hpp>

#include "application/gui/Piece.hpp"

#include "application/manager/TextureManager.hpp"

#include "utility/BoardUtility.hpp"

using namespace engine::board;

using namespace application::manager;

using namespace utility;

namespace application::gui {

Piece::Piece() : _piece(PieceType::EMPTY), _colour(ColourType::NONE) {
}

Piece::Piece(PieceType piece, ColourType colour) : _piece(piece), _colour(colour) {
}

void Piece::setPiece(PieceType piece) {
    this->_piece = piece;
}

void Piece::setColour(ColourType colour) {
    this->_colour = colour;
}

bool Piece::isEmpty() {
    return this->_piece == PieceType::EMPTY;
}

void Piece::clear() {
    this->_piece = PieceType::EMPTY;

    this->_colour = ColourType::NONE;
}

void Piece::render(float x, float y, float squareSize, sf::RenderWindow &window) {
    if (this->_piece == PieceType::EMPTY) {
        return;
    }

    sf::Sprite sprite = this->getSprite(x, y, squareSize);

    window.draw(sprite);
}

sf::Sprite Piece::getSprite(float x, float y, float squareSize) {
    int textureId = BoardUtility::getPieceIndex(this->_piece, this->_colour);

    sf::Texture &texture = TextureManager::getInstance().getTexture(textureId);

    sf::Sprite sprite(texture);

    sprite.setPosition(sf::Vector2f(x, y));

    sprite.setScale(sf::Vector2f(squareSize / texture.getSize().x, squareSize / texture.getSize().y));

    return sprite;
}

} // namespace application::gui
