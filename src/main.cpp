#include <iostream>

#include "game.h"

int main(int argc, char* argv[]) {
    Game game("configs/game.cg");
    game.startGame();

    return 0;
}