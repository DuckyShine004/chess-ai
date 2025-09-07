#include "application/gui/Chess.hpp"

namespace application::gui {

Chess::Chess() {
}

void Chess::render(sf::RenderWindow &window) {
    this->_board.render(window);
}

} // namespace application::gui