#pragma once

#include <SFML/Graphics.hpp>

#include "application/gui/Piece.hpp"
#include "application/gui/Square.hpp"

#include "engine/Engine.hpp"

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

namespace application::gui {

inline constexpr float OFFSET = 300.0f;

inline constexpr float PROMOTION_BORDER_THICKNESS = -2.5f;

inline constexpr sf::Color PROMOTION_BORDER_COLOUR = sf::Color(22, 21, 18);

inline constexpr engine::board::PieceType PROMOTION_PIECES[4] = {
    engine::board::PieceType::QUEEN,
    engine::board::PieceType::KNIGHT,
    engine::board::PieceType::ROOK,
    engine::board::PieceType::BISHOP,
};

struct PromotionSquare {
    float x;
    float y;

    float size;

    application::gui::Piece piece;

    sf::RectangleShape square;

    PromotionSquare(int id, engine::board::PieceType piece) : size(SIZE) {
        this->piece = Piece(piece, engine::board::ColourType::WHITE);

        this->x = (float)(id % 2) * SIZE + OFFSET;
        this->y = (float)(id / 2) * SIZE + OFFSET;

        this->square = sf::RectangleShape(sf::Vector2f(SIZE, SIZE));

        this->square.setPosition(sf::Vector2f(this->x, this->y));

        this->square.setFillColor(sf::Color::Transparent);

        this->square.setOutlineThickness(PROMOTION_BORDER_THICKNESS);

        this->square.setOutlineColor(PROMOTION_BORDER_COLOUR);
    }

    bool isCollideWithPoint(sf::Vector2i position) {
        sf::FloatRect bounds = this->square.getGlobalBounds();

        sf::Vector2f fPosition(position.x, position.y);

        return bounds.contains(fPosition);
    }

    void render(sf::RenderWindow &window) {
        window.draw(square);

        this->piece.render(this->x, this->y, this->size, window);
    }
};

class Promotion {
  public:
    Promotion();

    bool isPromoting();

    void setMove(uint16_t move);

    void setIsPromoting(bool isPromoting);

    void makePromotionMove(engine::Engine &engine, sf::Vector2i mousePosition);

    void render(sf::RenderWindow &window);

  private:
    int _from;
    int _to;

    bool _isCapture;
    bool _isPromoting;

    sf::RectangleShape _background;

    PromotionSquare *_promotionSquares[4];

    PromotionSquare *getPromotionSquare(sf::Vector2i mousePosition);

    void clear();

    void renderBackground(sf::RenderWindow &window);

    void renderBorder(sf::RenderWindow &window);
};

} // namespace application::gui
