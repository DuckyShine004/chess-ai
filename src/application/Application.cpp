#include <iostream>

#include "application/Application.hpp"

namespace application {

Application::Application() {
    sf::VideoMode mode(sf::Vector2u(1440, 900));

    this->_window = sf::RenderWindow(mode, "Chess");
}

void Application::run() {
    // this->_engine.run();

    // for (int depth = 1; depth <= 5; ++depth) {
    //     this->_engine.runPerft(depth);
    // }

    while (this->_window.isOpen()) {
        while (const std::optional event = this->_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                this->_window.close();
            }
        }

        this->update();
        this->render();
    }
}

void Application::update() {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(this->_window);
}

void Application::render() {
    this->_window.clear(sf::Color(22, 21, 18));

    this->_chess.render(this->_window);

    this->_window.display();
}

} // namespace application