#ifndef WARSHIPS_GAME_H
#define WARSHIPS_GAME_H

#include <string>
#include <vector>

#include "gamerules.h"
#include "standards.h"
#include "user.h"
#include "ui.h"

class Game {
private:
    GameRules _rules;
    ConsoleUI _ui;
    std::vector<Player> _players;
    Computer _computer;
    std::string _filename;

    short openMenu();
    void buildGame(bool isFullBuild = false);
    void prepareToGame();
    short startGame();
    short openOptions();
    short openTitles();
public:
    Game();
    Game(std::string filename);
    void launcher();
};

#endif // WARSHIPS_GAME_H
