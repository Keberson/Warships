#include <iostream>
#include <sstream>

#include "game.h"

// ConsoleUI                                                                TODO(danza_): сделать консольный UI

/*class Estream {
private:
    string _str;
public:
    Estream(string str): str(_str) {};

    friend ostream& operator <<(ostream& out, Estream estr);

};*/

int ConsoleUI::createScreen(unsigned width, unsigned height) {
    return 0;
}

int ConsoleUI::changeCellState(unsigned x, unsigned y, int state) {
    return 0;
}

int ConsoleUI::outputResult() {
    return 0;
}

int ConsoleUI::createField(unsigned width, unsigned height) {
    /*
    unsigned thickness = 2;     //
    unsigned rowWidth = thickness * 10 + thickness * 12;
    unsigned colWidth = thickness * 10 + thickness * 12;
    unsigned offset = 5;

    unsigned i;
    unsigned j;
    for (i = 1; i < colWidth; i++) {
        for (j = 1; j < rowWidth; j++) {
            if (((i / thickness) % thickness == 0) && ((j / thickness) % thickness == 0) &&
                    ((i / thickness) * (j / thickness)) != 0) {
                std::ostringstream ss;
                ss << "\033[" << i << ';' << j << "H\033[40m ";
                std::cout << ss.str();
            } else {
                std::ostringstream ss;
                ss << "\033[" << i << ';' << j << "H\033[47m ";
                std::cout << ss.str();
            }
        }
    }

    std::ostringstream ss;
    ss << "\033[" << i << ";0H\033[40m ";
    std::cout << ss.str();*/
    return 1;
}

void ConsoleUI::clearScreen() {
    std::cout << "\033[2J";
}

// /ConsoleUI

// Game
Game::Game(std::string filename) {
    _rules = GameRules((filename.empty()) ? STANDARD_CONFIG_PATH : filename );
    _ui = ConsoleUI();
    _players = { Player("Player1", _rules.getWidthField(), _rules.getHeightField()) };      // TODO(keberson): решить, как выбирать, кто будет играть
    _computer = Computer("Computer", _rules.getWidthField(), _rules.getHeightField());
}

void Game::startGame() {
    _ui.clearScreen();
    // TODO(keberson): создание экранов для нескольких пользователей
    _ui.createScreen(_rules.getWidthField(), _rules.getHeightField());
    for (int i = 0; i < _rules.getNumberOfShips(); ++i) {           // TODO(keberson): доделать размещение кораблей, не все корабли почему-то помещаются
        _players[0].placeShip(STANDARD_ID_START + i, _rules);
    }
    _players[0].outputField();

}
