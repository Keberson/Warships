#include <iostream>

#include "game.h"

int main(int argc, char* argv[]) {
    srand(time( nullptr ));
    Game game("configs/game.cg");
    game.prepareToGame();
    game.startGame();

    return 0;
}