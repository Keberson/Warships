#include <csignal>
#include <fstream>
#include <iostream>
#include <unistd.h>

#include "game.h"
#include "parser.h"
#include "standards.h"

#define FILENAME_SAVE_OPTIONS "configs/game.cg"

#define SIGNAL_MENU 1
#define SIGNAL_START_GAME 2
#define SIGNAL_OPTIONS 3
#define SIGNAL_TITLES 4
#define SIGNAL_EXIT 5

std::vector<std::string> OPTIONS_DESCRIPTION {"None", "None", "None", "rules:width", "rules:height", "None", "None", "None"};
std::vector<std::string> OPTIONS_RATIOS = {"", "", "", std::to_string(STANDARD_FIELD_WIDTH),
                                            std::to_string(STANDARD_FIELD_HEIGHT), "", "", ""};
std::vector<std::string> OPTIONS_RATIOS_MIN = {"", "", "", std::to_string(OPTIONS_WIDTH_MIN),
                                               std::to_string(OPTIONS_HEIGHT_MIN), "", "", ""};
std::vector<std::string> OPTIONS_RATIOS_MAX = {"", "", "", std::to_string(OPTIONS_WIDTH_MAX),
                                               std::to_string(OPTIONS_HEIGHT_MAX), "", "", ""};

void saveToFile(std::string filename) {
    std::fstream saveFile(filename);
    if (saveFile.is_open()) {
        for (int i = 0; i < OPTIONS_DESCRIPTION.size(); ++i) {
            saveFile.seekg(0);
            if (OPTIONS_DESCRIPTION[i] != "None") {
                bool isNecessary = false;
                bool isFounded = false;
                std::string line;
                std::string copyLine;
                std::string description = OPTIONS_DESCRIPTION[i];
                std::string chapter = description.substr(0, description.find(':'));
                std::string rule = description.substr(description.find(':') + 1);

                while (getline(saveFile, line) && !isFounded) {
                    copyLine = line;
                    auto commentPos = line.find("//");
                    if (commentPos != std::string::npos) {
                        line = line.substr(0, commentPos);
                    }

                    auto symbolPos = line.find(':');
                    if (symbolPos != std::string::npos) {
                        auto beforeSymbol = line.substr(0, symbolPos);
                        auto afterSymbol = line.substr(symbolPos + 1);
                        beforeSymbol = deleteSymbols(beforeSymbol, "\t \r\n");
                        afterSymbol = deleteSymbols(afterSymbol, "\t \r\n");
                        if (beforeSymbol == chapter) {
                            isNecessary = true;
                        }

                        if (isNecessary && beforeSymbol == rule) {
                            isFounded = true;
                            copyLine = deleteSymbols(copyLine, "\r\n");
                            saveFile.seekg((unsigned)saveFile.tellg() - copyLine.length() - 2);
                            unsigned length = copyLine.length();
                            copyLine.replace(copyLine.find(afterSymbol), afterSymbol.length(), OPTIONS_RATIOS[i]);
                            if (length > copyLine.length()) {
                                for (int j = 0; j < (length - copyLine.length()); ++j) {
                                    copyLine += " ";
                                }
                            }
                            saveFile << copyLine;
                        }
                    }

                    if (line == "~~~") {
                        isNecessary = false;
                    }
                }
            }
        }
    }

    saveFile.close();
    // TODO(keberosn): запись определенным образом, соответствующим парсеру
}

void emergencyInterruption(int p) {
    std::cout << "\033[?25h" << std::endl;      // Only for ConsoleUI
    saveToFile(FILENAME_SAVE_OPTIONS);
    exit(3);
}

Game::Game() {
    buildGame();
}

Game::Game(std::string filename) {
    _filename = filename;
    buildGame();
}

void Game::buildGame() {
    _rules = GameRules((_filename.empty()) ? STANDARD_CONFIG_PATH : _filename );
    ID_SHIPS_OFFSET = _rules.getNumberOfShips() / 10 + ((_rules.getNumberOfShips() % 10 == 0) ? 0 : 1) * 10;
    loadFromFile();
    _ui = ConsoleUI();
    _ui.synchronizeOptionsRatio(OPTIONS_RATIOS);
    _ui.setInputMode();            // Only for ConsoleUI
    _ui.buildMenu();
}


void Game::prepareToGame() {
    _ui.clearScreen();

    _players = { Player("Player1", _rules.getWidthField(), _rules.getHeightField()) };
    _computer = Computer("Computer", _rules.getWidthField(), _rules.getHeightField());

    for (int i = _rules.getNumberOfShips() - 1; i >= 0; --i) {
        _players[0].placeShip(STANDARD_ID_START + i, _rules);
    }

    for (int i = _rules.getNumberOfShips() - 1; i >= 0; --i) {
        _computer.placeShip(STANDARD_ID_START + i, _rules);
    }

    _rules.setWidthField(10);
    _rules.setHeightField(10);
}

short Game::startGame() {
    std::string winner;
    bool isGameEnd = false;
    bool isCanTurn = true;
    _ui.clearScreen();

    while (!isGameEnd) {
        // TODO(keberson): Задача для 3ий версии: реализация для случая, если 2 игрока
        // For ConsoleUI
        _ui.displayFields(_players[0].getField(), _computer.getField());

        isCanTurn = true;

        while (isCanTurn) {
            short turnSignal = _players[0].turn(_computer.getField(), _ui);
            if (turnSignal == SIGNAL_GAME_EXIT) {
                return SIGNAL_MENU;
            }

            isCanTurn = (turnSignal == SIGNAL_TURN_HIT);

            _ui.displayFields(_players[0].getField(), _computer.getField());
            sleep(1);

            if (_computer.getField().getNumberOfHits() == _rules.getSquareOfShips()) {
                isGameEnd = true;
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
    sleep(5);
    return SIGNAL_MENU;
}

short Game::openMenu() {
    unsigned rowCounter = _ui.getMenuStartIndex();
    unsigned prevRow;
    bool isSelected = false;
    bool isEscape = false;
    bool isArrows = false;
    char c;

    _ui.clearScreen();

    while (!isSelected) {
        _ui.menuDoRowActive("menu", rowCounter);
        _ui.displayMenu();

        c = getchar();

        if (c == '\033') {
            isEscape = true;
        }

        if (isEscape && c == '[') {
            isArrows = true;
        }

        if (isArrows) {
            prevRow = rowCounter;
            if (c == 'B') {
                if (rowCounter + 1 < _ui.getMenuSize() - 1) {
                    rowCounter++;
                } else {
                    rowCounter = _ui.getMenuStartIndex();
                }

                _ui.menuDoRowInactive("menu", prevRow);
                _ui.menuDoRowActive("menu", rowCounter);
            }

            if (c == 'A') {
                if (rowCounter - 1 >= _ui.getMenuStartIndex()) {
                    rowCounter--;
                } else {
                    rowCounter = _ui.getMenuSize() - 2;
                }

                _ui.menuDoRowInactive("menu", prevRow);
                _ui.menuDoRowActive("menu", rowCounter);
            }
        }

        if (c == '\n') {
            isSelected = true;
            _ui.menuDoRowInactive("menu", rowCounter);
        }
    }

    rowCounter -= _ui.getMenuStartIndex();
    switch (rowCounter) {
        case 0:
            return SIGNAL_START_GAME;
        case 1:
            return SIGNAL_OPTIONS;
        case 2:
            return SIGNAL_TITLES;
        case 3:
        default:
            return SIGNAL_EXIT;
    }
}

short Game::openOptions() {
    unsigned rowCounter = _ui.getOptionsStartIndex();
    unsigned prevRow;
    bool isEnd = false;
    bool isEscape = false;
    bool isArrows = false;
    char c;

    _ui.clearScreen();

    while (!isEnd) {
        _ui.menuDoRowActive("options", rowCounter);
        _ui.displayOptions();

        c = getchar();

        if (c == 'q') {
            isEnd = true;
            _ui.menuDoRowInactive("options", rowCounter);
        }

        if (c == '\033') {
            isEscape = true;
        }

        if (isEscape && c == '[') {
            isArrows = true;
        }

        if (isArrows) {
            prevRow = rowCounter;
            if (c == 'B') {
                if (rowCounter + 1 < _ui.getOptionsSize() - 1) {
                    rowCounter++;
                } else {
                    rowCounter = _ui.getOptionsStartIndex();
                }

                _ui.menuDoRowInactive("options", prevRow);
                _ui.menuDoRowActive("options", rowCounter);
            }

            if (c == 'A') {
                if (rowCounter - 1 >= _ui.getOptionsStartIndex()) {
                    rowCounter--;
                } else {
                    rowCounter = _ui.getOptionsSize() - 2;
                }

                _ui.menuDoRowInactive("options", prevRow);
                _ui.menuDoRowActive("options", rowCounter);
            }
        }

        if (c == '\n') {
            unsigned tempValue = rowCounter - _ui.getMenuStartIndex();
            if (tempValue == 0 || tempValue == 1) {
                _ui.menuDoValueActive(rowCounter);
                _ui.displayOptions();
                bool isSet = false;
                unsigned ratio = atoi(OPTIONS_RATIOS[rowCounter].c_str());
                while (!isSet) {
                    _ui.displayOptions();

                    c = getchar();

                    if (c == '\n') {
                        isSet = true;
                    }

                    if (c == '\033') {
                        isEscape = true;
                    }

                    if (isEscape && c == '[') {
                        isArrows = true;
                    }

                    if (isArrows) {
                        if (c == 'B') {
                            if (ratio - 1 >= atoi(OPTIONS_RATIOS_MIN[rowCounter].c_str())) {
                                ratio--;
                            } else {
                                ratio = atoi(OPTIONS_RATIOS_MAX[rowCounter].c_str());
                            }
                        }

                        if (c == 'A') {
                            if (ratio + 1 <= atoi(OPTIONS_RATIOS_MAX[rowCounter].c_str())) {
                                ratio++;
                            } else {
                                ratio = atoi(OPTIONS_RATIOS_MIN[rowCounter].c_str());
                            }
                        }

                        OPTIONS_RATIOS[rowCounter] = std::to_string(ratio);
                        _ui.setOptionsRatio(rowCounter, std::to_string(ratio));
                    }
                }
                isEscape = false;
                isArrows = false;
                _ui.menuDoValueInactive(rowCounter);
            } else {
                isEnd = true;
                _ui.menuDoRowInactive("options", prevRow);
            }
         }
    }

    return SIGNAL_MENU;
}

short Game::openTitles() {
    _ui.clearScreen();
    _ui.displayTitles();
    _ui.clearScreen();
    return SIGNAL_MENU;
}

void Game::loadFromFile() {   // TODO(keberson): не робит, починит
    OPTIONS_RATIOS[3] = "10";//std::to_string(_rules.getWidthField());
    OPTIONS_RATIOS[4] = "10";std::to_string(_rules.getHeightField());
}

void Game::launcher() {
    bool isExit = false;
    short chapter = SIGNAL_MENU;
    signal(SIGINT, emergencyInterruption);

    while (!isExit) {
        switch (chapter) {
            case SIGNAL_MENU:
                chapter = openMenu();
                break;
            case SIGNAL_START_GAME:
                prepareToGame();
                chapter = startGame();
                break;
            case SIGNAL_OPTIONS:
                chapter = openOptions();
                break;
            case SIGNAL_TITLES:
                chapter = openTitles();
                break;
            case SIGNAL_EXIT:
                isExit = true;
        }
    }

    _ui.clearScreen();
    std::cout << "\033[?25h" << std::endl;      // Only for ConsoleUI
    saveToFile(FILENAME_SAVE_OPTIONS);
    exit(1);
}
