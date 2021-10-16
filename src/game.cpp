#include <csignal>
#include <iostream>
#include <unistd.h>

#include "game.h"

void emergencyInterruption(int p) {
    std::cout << "\033[?25h" << std::endl;
    exit(3);
}

Game::Game(std::string filename) {
    _rules = GameRules((filename.empty()) ? STANDARD_CONFIG_PATH : filename );
    _ui = ConsoleUI();
    _players = { Player("Player1", _rules.getWidthField(), _rules.getHeightField()) };      // TODO(keberson): решить, как выбирать, кто будет играть
    _computer = Computer("Computer", _rules.getWidthField(), _rules.getHeightField());
}

void Game::prepareToGame() {
    std::cout << "\033[?25l";
    _ui.clearScreen();
    // TODO(keberson): создание экранов для нескольких пользователей
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
        // TODO(keberson): реализация для случая, если 2 игрока

        _ui.displayFields(_players[0].getField(0), _players[0].getField(1));

        while (isCanTurn) {
            isCanTurn = _players[0].turn(_computer.getField(0), _rules.getNumberOfShips());
            _ui.displayFields(_players[0].getField(0), _players[0].getField(1));
            sleep(1);

            if (_players[0].getField(1).getNumberOfHits() == _rules.getSquareOfShips()) {
                isGameEnd = true;
                winner = "Player";
                break;
            }
        }

        if (isGameEnd) {
            winner = "Player";
            break;
        }

        isCanTurn = true;

        _ui.displayFields(_players[0].getField(0), _players[0].getField(1));
        std::cout << "Computer is attacking" << std::endl;
        sleep(1);

        while (_computer.turn(_players[0].getField(0), _rules.getNumberOfShips())) {
            _ui.displayFields(_players[0].getField(0), _players[0].getField(1));
            std::cout << "Computer is attacking again" << std::endl;
            sleep(1);

            if (_players[0].getField(0).getNumberOfHits() == _rules.getSquareOfShips()) {
                isGameEnd = true;
                winner = "Computer";
                break;
            }
        }

        isCanTurn = true;
    }

    std::cout << std::endl << "Congratulations! " << winner << " is winner!" << std::endl;
}