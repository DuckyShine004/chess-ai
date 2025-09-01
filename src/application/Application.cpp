#include "application/Application.hpp"

#include "engine/board/Board.hpp"

using namespace engine::board;

namespace application {

Application::Application() {
    // std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR - - - - -";
    std::string fen = "8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b - - 99 50";

    this->_board = Board(fen);
}

void Application::run() {
    this->_board.print();
}

} // namespace application