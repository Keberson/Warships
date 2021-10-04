#include <iostream>
#include <sstream>
#include <unistd.h>

#include "game.h"

// ConsoleUI                                                                TODO(danza_): сделать консольный UI

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

std::string ConsoleUI::getCell() {
    std::string tempStr;
    std::cin >> tempStr;
    return tempStr;
}

// /ConsoleUI

// Game
Game::Game(std::string filename) {
    _rules = GameRules((filename.empty()) ? STANDARD_CONFIG_PATH : filename );
    _ui = ConsoleUI();
    _players = { Player("Player1", _rules.getWidthField(), _rules.getHeightField()) };      // TODO(keberson): решить, как выбирать, кто будет играть
    _computer = Computer("Computer", _rules.getWidthField(), _rules.getHeightField());
}

void Game::prepareToGame() {
    _ui.clearScreen();
    // TODO(keberson): создание экранов для нескольких пользователей
    _ui.createScreen(_rules.getWidthField(), _rules.getHeightField());
    for (int i = _rules.getNumberOfShips() - 1; i >= 0; --i) {
        _players[0].placeShip(STANDARD_ID_START + i, _rules);
    }

    std::cout << "It's your field" << std::endl;
    std::cout << _players[0].getField(0) << std::endl;

    for (int i = _rules.getNumberOfShips() - 1; i >= 0; --i) {
        _computer.placeShip(STANDARD_ID_START + i, _rules);
    }

    sleep(5);

    // TODO(keberson): вывод двух полей в случае, если противники Человек и Компьютер
}

void Game::startGame() {
    bool isGameEnd = false;
    while (!isGameEnd) {
        // TODO(keberson): реализация для случая, если 2 игрока


        // TODO(keberson): условие окончание игры, добавить буквы и цифры сверху и справа соответственно
        std::cout << std::endl << "Now is your turn to attack" << std::endl;
        std::cout << _players[0].getField(1) << std::endl;
        _players[0].turn(_computer.getField(0), _rules.getNumberOfShips());
        std::cout << std::endl << "Result of your turn" << std::endl;
        std::cout << _players[0].getField(1) << std::endl;
        sleep(5);
        /*if (_players[0].getField(0).getNumberOfHits() == _rules.getSquareOfShips()) {
            isGameEnd = true;
        }*/

        std::cout << std::endl << "Computer is attacking" << std::endl;
        std::cout << _players[0].getField(0) << std::endl;
        sleep(2);

        _computer.turn(_players[0].getField(0), _rules.getNumberOfShips());
        std::cout << std::endl << "Computer is attacking" << std::endl;
        std::cout << _players[0].getField(0) << std::endl;
        sleep(5);
    }

    _ui.clearScreen();
}