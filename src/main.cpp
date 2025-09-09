#include "application/Application.hpp"

using namespace application;

int main(int argc, char *argv[]) {
    Application application;

    application.initialise();

    // clang-format off
    #ifdef NDEBUG
        application.run();
    #else
        application.debug();
    #endif
    // clang-format on

    return 0;
}
