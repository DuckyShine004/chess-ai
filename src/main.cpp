#include "application/Application.hpp"

using namespace application;

int main(int argc, char *argv[]) {
    Application application;

    // clang-format off
    #ifdef NDEBUG
        application.initialise();

        application.run();
    #else
        application.debug();
    #endif
    // clang-format on

    return 0;
}
