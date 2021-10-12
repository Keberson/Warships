#include <iostream>
#include <sstream>
#include <unistd.h>

#include "game.h"

Game::Game(std::string filename) {
    _rules = GameRules((filename.empty()) ? STANDARD_CONFIG_PATH : filename );
    _ui = ConsoleUI();
    _players = { Player("Player1", _rules.getWidthField(), _rules.getHeightField()) };      // TODO(keberson): решить, как выбирать, кто будет играть
    _computer = Computer("Computer", _rules.getWidthField(), _rules.getHeightField());
}

void Game::prepareToGame() {
    _ui.clearScreen();
    // TODO(keberson): создание экранов для нескольких пользователей
    _ui.displayFields(_rules.getWidthField(), _rules.getHeightField());
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

        std::cout << std::endl << "Now is your turn to attack" << std::endl;
        std::cout << _players[0].getField(1) << std::endl;
        while (_players[0].turn(_computer.getField(0), _rules.getNumberOfShips())) {
            std::cout << std::endl << "Your turn to attack again" << std::endl;
            std::cout << _players[0].getField(1) << std::endl;
        }

        std::cout << std::endl << "Result of your turn" << std::endl;
        std::cout << _players[0].getField(1) << std::endl;
        sleep(5);
        if (_computer.getField(0).getNumberOfHits() == _rules.getSquareOfShips()) {
            isGameEnd = true;
        }

        std::cout << std::endl << "Computer is attacking" << std::endl;
        std::cout << _players[0].getField(0) << std::endl;
        sleep(2);

        while (_computer.turn(_players[0].getField(0), _rules.getNumberOfShips())) {
            std::cout << std::endl << "Computer is attacking again" << std::endl;
            std::cout << _players[0].getField(0) << std::endl;
            sleep(2);
        }

        std::cout << std::endl << "Computer attacked" << std::endl;
        std::cout << _players[0].getField(0) << std::endl;
        sleep(5);
        if (_players[0].getField(0).getNumberOfHits() == _rules.getSquareOfShips()) {
            isGameEnd = true;
        }
    }

    _ui.clearScreen();
}