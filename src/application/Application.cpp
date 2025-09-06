#include "application/Application.hpp"

namespace application {

Application::Application() {
}

void Application::run() {
    this->_engine.parse("rn1qkbnr/pppppppp/2bP4/3N4/B3K3/8/PPP1PPPP/RNBQ3R w KQkq - 0 1");

    this->_engine.printBoard();

    this->_engine.run();
}

} // namespace application