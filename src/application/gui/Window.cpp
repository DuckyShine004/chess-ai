#include "application/gui/Window.hpp"

namespace application::gui {

Window::Window() : _width(this->_WIDTH), _height(this->_HEIGHT) {
    // this->_mode = sf::VideoMode(sf::Vector2u(this->_WIDTH, this->_HEIGHT));

    // this->_window = sf::RenderWindow(this->_mode, "SFML 3");

    // while (this->_window.isOpen()) {
    //     while (const std::optional event = this->_window.pollEvent()) {
    //         if (event->is<sf::Event::Closed>()) {
    //             this->_window.close();
    //         }
    //     }
    //     this->_window.clear();
    //     this->_window.display();
    // }
}

} // namespace application::gui