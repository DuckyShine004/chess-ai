#include "application/gui/Board.hpp"

#include "engine/board/Colour.hpp"

#include "utility/BoardUtility.hpp"

using namespace engine::board;

using namespace utility;

namespace application::gui {

Board::Board() {
    for (int square = 0; square < 64; ++square) {
        int rank = BoardUtility::getRank(square);
        int file = BoardUtility::getFile(square);

        ColourType colour;

        if (rank & 1) {
            if (file & 1) {
                colour = ColourType::BLACK;
            } else {
                colour = ColourType::WHITE;
            }
        } else {
            if (file & 1) {
                colour = ColourType::WHITE;
            } else {
                colour = ColourType::BLACK;
            }
        }

        this->_squares[square] = Square(rank, file, colour);
    }
}

void Board::render(sf::RenderWindow &window) {
    for (Square &square : this->_squares) {
        square.render(window);
    }
}

} // namespace application::gui