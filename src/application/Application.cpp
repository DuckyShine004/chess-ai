#include "application/Application.hpp"

namespace application {

Application::Application() {
}

void Application::run() {
    this->_engine.parse("rnbqkbnr/pppppppp/2KP4/5N2/8/4B3/PPP1PPPP/RNBQ3R w KQkq - 0 1");

    this->_engine.printBoard();

    this->_engine.run();
}

} // namespace application