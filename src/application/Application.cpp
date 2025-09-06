#include "application/Application.hpp"

namespace application {

Application::Application() {
}

void Application::run() {
    this->_engine.parse("rnbqkbnr/ppppp1pp/3P4/8/8/5p2/PPP1PPPP/RNBQKBNR w KQkq - 0 1");

    this->_engine.printBoard();

    this->_engine.run();
}

} // namespace application