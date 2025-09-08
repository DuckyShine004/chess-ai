#include "application/gui/Chess.hpp"

#include "engine/board/Piece.hpp"

using namespace engine::board;

namespace application::gui {

Chess::Chess() {
}

// Move generation is practically O(1) perhaps memory optimisations could be made...
void Chess::move(ColourType side) {
    for (uint8_t piece = PieceType::PAWN; piece <= PieceType::KING; ++piece) {
    }
}

void Chess::render(sf::RenderWindow &window) {
    this->_board.render(window);
}

} // namespace application::gui