#include "application/gui/Square.hpp"
#include "application/gui/Promotion.hpp"

#include "engine/move/Move.hpp"

using namespace engine;

using namespace engine::board;

using namespace engine::move;

namespace application::gui {

Promotion::Promotion() : _from(-1), _to(-1), _isPromoting(false) {
    for (int id = 0; id < 4; ++id) {
        this->_promotionSquares[id] = new PromotionSquare(id, PROMOTION_PIECES[id]);
    }

    this->_background = sf::RectangleShape(sf::Vector2f(SIZE * 2, SIZE * 2));

    this->_background.setPosition(sf::Vector2f(OFFSET, OFFSET));

    this->_background.setFillColor(sf::Color::White);
}

bool Promotion::isPromoting() {
    return this->_isPromoting;
}

void Promotion::setFrom(int from) {
    this->_from = from;
}

void Promotion::setTo(int to) {
    this->_to = to;
}

void Promotion::setIsPromoting(bool isPromoting) {
    this->_isPromoting = isPromoting;
}

void Promotion::makePromotionMove(Engine &engine, sf::Vector2i mousePosition) {
    PromotionSquare *promotionSquare = this->getPromotionSquare(mousePosition);

    if (promotionSquare == nullptr) {
        return;
    }

    PieceType promotionPiece = promotionSquare->piece.getPiece();

    uint16_t promotionMove = Move::getMove(this->_from, this->_to, promotionPiece);

    engine.makeMove(promotionMove);

    this->_isPromoting = false;
}

PromotionSquare *Promotion::getPromotionSquare(sf::Vector2i mousePosition) {
    for (PromotionSquare *promotionSquare : this->_promotionSquares) {
        if (promotionSquare->isCollideWithPoint(mousePosition)) {
            return promotionSquare;
        }
    }

    return nullptr;
}
void Promotion::renderBackground(sf::RenderWindow &window) {
    window.draw(this->_background);
}

void Promotion::renderBorder(sf::RenderWindow &window) {
    sf::RectangleShape border = this->_background;

    border.setFillColor(sf::Color::Transparent);

    border.setOutlineColor(BORDER_COLOUR);

    border.setOutlineThickness(BORDER_THICKNESS);

    window.draw(border);
}

void Promotion::render(sf::RenderWindow &window) {
    if (!this->_isPromoting) {
        return;
    }

    window.draw(this->_background);

    for (PromotionSquare *promotionSquare : this->_promotionSquares) {
        promotionSquare->render(window);
    }

    this->renderBorder(window);
}

} // namespace application::gui
