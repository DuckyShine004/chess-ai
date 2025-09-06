#include "application/Application.hpp"

namespace application {

Application::Application() {
}

void Application::run() {
    this->_engine.parse("N6R/2NK1p2/1p4b1/P3P3/1b6/1P1k2Pp/p4B2/8 w - - 0 1");

    this->_engine.printBoard();

    this->_engine.run();
}

} // namespace application