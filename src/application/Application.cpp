#include <SFML/Window/Mouse.hpp>

#include "application/Application.hpp"

#include "application/manager/SoundManager.hpp"
#include "application/manager/TextureManager.hpp"

#include "engine/board/Fen.hpp"

#include "sound/SoundPlayer.hpp"

#include "logger/LoggerMacros.hpp"

using namespace engine::board;

using namespace application::manager;

using namespace sound;

namespace application {

Application::Application() {
}

void Application::initialiseRenderer() {
    sf::VideoMode mode(sf::Vector2u(1440, 900));

    this->_window = sf::RenderWindow(mode, "Chess");

    sf::ContextSettings settings;

    settings.antiAliasingLevel = 8;
}

void Application::initialise() {
    // this->_engine.parse(PROMOTION_POSITIONS[0]);
    // this->_engine.parse(KILLER_POSITION);
    // this->_engine.parse(EN_PASSANT_POSITIONS[0]);
    this->_engine.parse(PUZZLE_POSITIONS[5]);
    // this->_engine.parse(NMP_POSITIONS[2]);
    // this->_engine.parse(REPETITION_POSITIONS[0]);
    // this->_engine.parse(ISOLATED_PAWN_POSITIONS[0]);
    // this->_engine.parse(PASSED_PAWN_POSITIONS[0]);
    // this->_engine.parse(SEMI_OPEN_FILE_POSITIONS[0]);

    this->initialiseRenderer();

    SoundPlayer::getInstance().initialise();

    SoundManager::getInstance().initialise();

    TextureManager::getInstance().initialise();
}

void Application::run() {
    while (this->_window.isOpen()) {
        while (const std::optional event = this->_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                this->_window.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                const auto &mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();

                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    this->_chess.move(this->_window, this->_engine, mouseEvent->position);
                }
            }
        }

        this->update();
        this->render();
    }
}

void Application::debug() {
    this->_engine.parse(POSITIONS[4]);

    this->_engine.printBoard();

    for (int depth = 1; depth <= 5; ++depth) {
        this->_engine.runPerft(depth);
    }
    // int moves = 20;
    // long long totalElapsed = 0LL;
    // for (int i = 0; i < moves; ++i) {
    //     auto start = std::chrono::high_resolution_clock::now();
    //     this->_engine.run();
    //     auto end = std::chrono::high_resolution_clock::now();
    //     auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     this->_engine.printBoard();
    //     totalElapsed += elapsed.count();
    // }
    // LOG_DEBUG("Moves: {}", moves);
    // LOG_DEBUG("Time: {} ms", totalElapsed);
    // this->_engine.parse(KILLER_POSITION);

    // this->_engine.printBoard();
    //
    // this->_engine.run();
    //
    // this->_engine.printBoard();
}

void Application::update() {
    this->_chess.update(this->_window, this->_engine);
}

void Application::render() {
    this->_window.clear(BACKGROUND_COLOUR);

    this->_chess.render(this->_window);

    this->_window.display();
}

} // namespace application
