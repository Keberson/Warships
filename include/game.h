// WARSHIPS_GAME_H

#ifndef WARSHIPS_GAME_H
#define WARSHIPS_GAME_H

#include <string>
#include <vector>

#include "gamerules.h"
#include "user.h"
#include "standards.h"

class IUI {
public:
    virtual int createScreen(unsigned, unsigned) = 0;
    virtual int createField(unsigned, unsigned) = 0;
    virtual int changeCellState(unsigned, unsigned, int) = 0;
    virtual int outputResult() = 0;
    virtual void clearScreen() = 0;
    virtual std::string getCell() = 0;
    virtual ~IUI() {};
};

class ConsoleUI: public IUI {
public:
    ConsoleUI() {};
    int createScreen(unsigned, unsigned);
    int createField(unsigned, unsigned);
    int changeCellState(unsigned, unsigned, int);
    int outputResult();
    void clearScreen();
    std::string getCell();
};

class Game {
private:
    GameRules _rules;
    ConsoleUI _ui;
    std::vector<Player> _players;
    Computer _computer;
public:
    Game(std::string filename = "");
    void prepareToGame();
    void startGame();
};

#endif // WARSHIPS_GAME_H
