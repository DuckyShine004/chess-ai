#include "application/Application.hpp"

using namespace application;

int main(int argc, char *argv[]) {
    Application application;

    application.initialise();

    application.run();

    return 0;
}