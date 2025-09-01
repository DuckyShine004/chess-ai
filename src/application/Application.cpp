#include "application/Application.hpp"

#include "engine/board/Board.hpp"

using namespace engine::board;

namespace application {

Application::Application() {
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

    this->_board = Board(fen);
}

} // namespace application