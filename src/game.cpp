#include <csignal>
#include <iostream>
#include <unistd.h>

#include "game.h"

void emergencyInterruption(int p) {
    std::cout << "\033[?25h" << std::endl;      // Only for ConsoleUI
    exit(3);
}

Game::Game(std::string filename) {
    _rules = GameRules((filename.empty()) ? STANDARD_CONFIG_PATH : filename );
    ID_SHIPS_OFFSET = _rules.getNumberOfShips() / 10 + ((_rules.getNumberOfShips() % 10 == 0) ? 0 : 1) * 10;
    _ui = ConsoleUI();
    _players = { Player("Player1", _rules.getWidthField(), _rules.getHeightField()) };      // TODO(keberson): Задача для 3ий версии: решить, как выбирать, кто будет играть
    _computer = Computer("Computer", _rules.getWidthField(), _rules.getHeightField());
}

void Game::prepareToGame() {
    _ui.setInputMode();            // Only for ConsoleUI
    _ui.clearScreen();

    // TODO(keberson): Задача для 3ий версии: создание экранов для нескольких пользователей

    for (int i = _rules.getNumberOfShips() - 1; i >= 0; --i) {
        _players[0].placeShip(STANDARD_ID_START + i, _rules);
    }

    for (int i = _rules.getNumberOfShips() - 1; i >= 0; --i) {
        _computer.placeShip(STANDARD_ID_START + i, _rules);
    }
}

void Game::startGame() {
    std::string winner;
    bool isGameEnd = false;
    bool isCanTurn = true;
    _ui.clearScreen();
    signal(SIGINT, emergencyInterruption);

    while (!isGameEnd) {
        // TODO(keberson): Задача для 3ий версии: реализация для случая, если 2 игрока
        // For ConsoleUI
        _ui.displayFields(_players[0].getField(), _computer.getField());

        isCanTurn = true;

        while (isCanTurn) {
            isCanTurn = _players[0].turn(_computer.getField(), _ui);
            _ui.displayFields(_players[0].getField(), _computer.getField());
            sleep(1);

            if (_computer.getField().getNumberOfHits() == _rules.getSquareOfShips()) {
                isGameEnd = true;
                winner = "Player";
                break;
            }
        }

        if (isGameEnd) {
            winner = "Player";
            break;
        }

        _ui.displayFields(_players[0].getField(), _computer.getField());
        std::cout << "Computer is attacking" << std::endl;
        sleep(1);

        while (_computer.turn(_players[0].getField(), _ui)) {
            _ui.displayFields(_players[0].getField(), _computer.getField());
            std::cout << "Computer is attacking again" << std::endl;
            sleep(1);

            if (_players[0].getField().getNumberOfHits() == _rules.getSquareOfShips()) {
                isGameEnd = true;
                winner = "Computer";
                break;
            }
        }
        // /For ConsoleUI
    }

    std::cout << std::endl << "Congratulations! " << winner << " is winner!" << std::endl;
}