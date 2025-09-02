#include <iostream>
#include <limits>

#include "application/Application.hpp"

using namespace application;

int main() {
    std::cout << "Hello world!" << std::endl;

    Application application;

    application.run();

    return 0;
}