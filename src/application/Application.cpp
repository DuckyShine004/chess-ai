#include "application/Application.hpp"

#include "engine/board/Board.hpp"

using namespace engine::board;

namespace application {

Application::Application() {
}

void Application::run() {
    this->_engine.run();
}

} // namespace application