#include <cstdlib>
#include <ctime>

#include "game.h"

int main(int argc, char* argv[]) {
    srand(time(nullptr));
    Game game;
    game.launcher();

    std::cout << "\033[?25h" << std::endl;
    return 0;
}