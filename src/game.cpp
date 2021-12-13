#include <csignal>
#include <fstream>
#include <iostream>
#include <unistd.h>

#include "game.h"
#include "parser.h"
#include "standards.h"

#define FILENAME_SAVE_OPTIONS "configs/game.cg"

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
                            auto copyLineFind = copyLine.find(afterSymbol);
                            if (copyLineFind != std::string::npos) {
                                copyLine.replace(copyLineFind, afterSymbol.length(), OPTIONS_RATIOS[i]);
                                if (length > copyLine.length()) {
                                    for (int j = 0; j < (length - copyLine.length()); ++j) {
                                        copyLine += " ";
                                    }
                                }

                                saveFile << copyLine;
                            }
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


short Game::prepareToGame() {
    _ui.clearScreen();

    _players = { Player("Player1", _rules.getWidthField(), _rules.getHeightField(), _rules.getShips()) };
    _computer = Computer("Computer", _rules.getWidthField(), _rules.getHeightField(), _rules.getShips());

    _ui.prepareShipSelect(_players[0].getField());

    bool isAllShipsPlaced = false;
    std::vector<Ship> allShips = _players[0].getField().getAllShips();
    unsigned placedShips = 0;
    unsigned notPlacedShips = allShips.size();
    char c;
    bool isEscape = false;
    bool isArrows = false;
    int currentRow = 0;
    std::vector<unsigned> inactiveRows;

    _ui.setShipDoRowActive(currentRow);
    _players[0].randomPlaceIsland(_players[0].getField().getWidth() * _players[0].getField().getHeight() / ISLAND_SQUARE - rand() % 10);
    while (!isAllShipsPlaced) {
        bool isSetupShip = false;
        _ui.displayShipsSelect(_players[0].getField());
        if (placedShips >= notPlacedShips) {
            isAllShipsPlaced = true;
        }

        c = _ui.getSymbol();

        if (c == 'q') {
            return SIGNAL_MENU;
        }

        if (c == 'p') {
            for (int i = 0; i < allShips.size(); ++i) {
                bool isFounded = false;
                for (auto item: inactiveRows) {
                    if (item == i) {
                        isFounded = true;
                        break;
                    }
                }

                if (!isFounded) {
                    _players[0].randomPlaceShip(allShips[i].getId());
                }
            }

            isAllShipsPlaced = true;
        }

        if (c == '\033') {
            isEscape = true;
        }

        if (isEscape && c == '[') {
            isArrows = true;
        }

        if (isArrows) {
            int prevRow = currentRow;
            if (c == 'B') {
                if (currentRow + 1 < _ui.getShipSelectSize()) {
                    currentRow++;
                } else {
                    currentRow = 0;
                }

                _ui.setShipDoRowInactive(prevRow);
                _ui.setShipDoRowActive(currentRow);
            }

            if (c == 'A') {
                if (currentRow - 1 >= 0) {
                    currentRow--;
                } else {
                    currentRow = _ui.getShipSelectSize() - 1;
                }

                _ui.setShipDoRowInactive(prevRow);
                _ui.setShipDoRowActive(currentRow);
            }
        }

        if (c == '\n') {
            bool isFounded = false;
            Field field = _players[0].getField();
            for (auto item: inactiveRows) {
                if (item == currentRow) {
                    isFounded = true;
                }
            }

            if (!isFounded) {
                bool isSet = false;
                bool isRotate = false;
                unsigned height = field.getHeight();
                unsigned width = field.getWidth();
                Ship currentShip = allShips[currentRow];
                unsigned shipLength = currentShip.getLength();
                unsigned shipWidth = currentShip.getWidth();
                std::vector<int> x;
                std::vector<int> y;

                for (int i = 0; i < shipWidth; ++i) {
                    x.push_back(i);
                }

                for (int i = 0; i < shipLength; ++i) {
                    y.push_back(i);
                }

                std::vector<Cell*> border;
                std::vector<Cell> prevID;

                while (!isSet) {
                    for (auto item: prevID) {
                        field.getCell(item.getX(), item.getY()).setID(item.getID());
                    }

                    prevID.clear();
                    for (int i = 0; i < y.size(); ++i) {
                        for (int j = 0; j < x.size(); ++j) {
                            prevID.push_back(field.getCell(x[j], y[i]));
                            field.setID(x[j], y[i], 5);
                        }
                    }

                    _ui.displayShipsSelect(field);
                    c = _ui.getSymbol();

                    if (c == 'q') {
                        isSet = true;
                    }

                    if (c == 'r') {
                        unsigned tempX = x[0];
                        unsigned tempY = y[0];
                        bool isCanRotate = false;

                        if (!isRotate) {
                            if (tempY + shipWidth <= height) {
                                isCanRotate = true;
                            }
                        } else {
                            if (tempX + shipLength <= width) {
                                isCanRotate = true;
                            }
                        }

                        if (isCanRotate) {
                            unsigned temp = shipWidth;
                            shipWidth = shipLength;
                            shipLength = temp;
                            x.clear();
                            y.clear();

                            for (int i = 0; i < shipWidth; ++i) {
                                x.push_back(tempX + i);
                            }

                            for (int i = 0; i < shipLength; ++i) {
                                y.push_back(tempY + i);
                            }

                            isRotate = !isRotate;
                        }
                    }

                    if (c == '\n') {
                        for (auto item: prevID) {
                            field.getCell(item.getX(), item.getY()).setID(item.getID());
                        }

                        isSet = true;
                        unsigned yPrevSize = y.size();
                        unsigned xPrevSize = x.size();
                        bool isTwoY = false;
                        bool isTwoX = false;

                        if (y[y.size() - 1] + 1 < field.getHeight()) {
                            y.push_back(y[y.size() - 1] + 1);
                        }

                        if (y[0] - 1 >= 0) {
                            y.push_back(y[0] - 1);
                        }

                        if (x[x.size() - 1] + 1 < width) {
                            x.push_back(x[x.size() - 1] + 1);
                        }

                        if (x[0] - 1 >= 0) {
                            x.push_back(x[0] - 1);
                        }

                        if (y.size() - yPrevSize == 2) {
                            isTwoY = true;
                        }

                        if (x.size() - xPrevSize == 2) {
                            isTwoX = true;
                        }

                        bool isNotCorrectCells = false;

                        for (int i = 0; i < y.size(); ++i) {
                            for (int j = 0; j < x.size(); ++j) {
                                if (field.getCell(x[j], y[i]).getID() / 10 == 1) {
                                    isNotCorrectCells = true;
                                    break;
                                }

                                if (isTwoY) {
                                    if (i == y.size() - 2) {
                                        border.push_back(&field.getCell(x[j], y[i]));

                                        continue;
                                    } else if (i != y.size() - 1 && j != x.size() - 1 && j != x.size() - 2) {
                                        if (field.getCell(x[j], y[i]).getID() == ISLAND_ID) {
                                            isNotCorrectCells = true;
                                            break;
                                        }
                                    }
                                }

                                if (isTwoX) {
                                    if (j == x.size() - 2) {
                                        border.push_back(&field.getCell(x[j], y[i]));

                                        continue;
                                    } else if (j != x.size() - 1 && i != y.size() - 1 && i != y.size() - 2) {
                                        if (field.getCell(x[j], y[i]).getID() == ISLAND_ID) {
                                            isNotCorrectCells = true;
                                            break;
                                        }
                                    }
                                }

                                if (i == y.size() - 1 || j == x.size() - 1) {
                                    border.push_back(&field.getCell(x[j], y[i]));
                                } else {
                                    if (field.getCell(x[j], y[i]).getID() == ISLAND_ID) {
                                        isNotCorrectCells = true;
                                        break;
                                    }
                                }
                            }

                            if (isNotCorrectCells) {
                                border.clear();
                                isSet = false;
                                break;
                            }
                        }

                        y.pop_back();
                        if (isTwoY) {
                            y.pop_back();
                        }

                        x.pop_back();
                        if (isTwoX) {
                            x.pop_back();
                        }

                        if (!isNotCorrectCells) {
                            for (int i = 0; i < y.size(); ++i) {
                                for (int j = 0; j < x.size(); ++j) {
                                    _players[0].getField().setID(x[j], y[i], currentShip.getId());
                                }
                            }

                            field.addShipBorder(currentShip.getId(), border);
                            isSetupShip = true;
                        }
                    }

                    if (c == '\033') {
                        isEscape = true;
                    }

                    if (isEscape && c == '[') {
                        isArrows = true;
                    }

                    if (isArrows) {
                        if (c == 'C') {
                            if (x[0] + shipWidth < width) {
                                for (int i = 0; i < x.size(); ++i) {
                                    x[i]++;
                                }
                            } else {
                                for (int i = 0; i < x.size(); ++i) {
                                    x[i] = i;
                                }
                            }
                        }

                        if (c == 'D') {
                            if (x[0] - 1 >= 0) {
                                for (int i = 0; i < x.size(); ++i) {
                                    x[i]--;
                                }
                            } else {
                                for (int i = 0; i < x.size(); ++i) {
                                    x[i] = (int)width - (int)shipWidth + i;
                                }
                            }
                        }

                        if (c == 'B') {
                            if (y[0] + shipLength < height) {
                                for (int i = 0; i < y.size(); ++i) {
                                    y[i]++;
                                }
                            } else {
                                for (int i = 0; i < y.size(); ++i) {
                                    y[i] = i;
                                }
                            }
                        }

                        if (c == 'A') {
                            if (y[0] - 1 >= 0) {
                                for (int i = 0; i < y.size(); ++i) {
                                    y[i]--;
                                }
                            } else {
                                for (int i = 0; i < y.size(); ++i) {
                                    y[i] = (int)height - (int)shipLength + i;
                                }
                            }
                        }
                    }
                }

                isEscape = false;
                isArrows = false;
            }

            if (isSetupShip) {
                _ui.setShipDoRowFilled(currentRow);
                _ui.setShipDoRowInactive(currentRow);
                inactiveRows.push_back(currentRow);
                if (inactiveRows.size() == _ui.getShipSelectSize()) {
                    isAllShipsPlaced = true;
                } else {
                    bool isFounded = false;
                    while (!isFounded) {
                        bool isIn = false;
                        for (auto item: inactiveRows) {
                            if (item == currentRow) {
                                isIn = true;
                                break;
                            }
                        }

                        if (!isIn) {
                            isFounded = true;
                            break;
                        }

                        if (currentRow + 1 < _ui.getShipSelectSize()) {
                            currentRow++;
                        } else {
                            currentRow = 0;
                        }
                    }

                    _ui.setShipDoRowActive(currentRow);
                }
            }
        }
    }

    _computer.randomPlaceIsland(_players[0].getField().getWidth() * _players[0].getField().getHeight() / ISLAND_SQUARE - rand() % 10);
    for (int i = _rules.getNumberOfShips() - 1; i >= 0; --i) {
        _computer.randomPlaceShip(STANDARD_ID_START + i);
    }

    return SIGNAL_START_GAME;
}

short Game::startGame() {
    std::string winner;
    bool isGameEnd = false;
    bool isCanTurn = true;
    _ui.clearScreen();

    while (!isGameEnd) {
        _ui.displayFields(_players[0].getField(), _computer.getField());

        isCanTurn = true;

        while (isCanTurn) {
            short turnSignal = _players[0].turn(_computer.getField(), _ui);
            if (turnSignal == SIGNAL_GAME_EXIT) {
                return SIGNAL_MENU;
            }

            sleep(1);
            isCanTurn = (turnSignal == SIGNAL_TURN_HIT);

            if (_computer.getField().getNumberOfHits() == _rules.getSquareOfShips()) {
                isGameEnd = true;
                break;
            }
        }

        if (isGameEnd) {
            winner = "Player";
            break;
        }

        sleep(1);

        while (_computer.turn(_players[0].getField(), _ui)) {
            if (_players[0].getField().getNumberOfHits() == _rules.getSquareOfShips()) {
                isGameEnd = true;
                winner = "Computer";
                break;
            }
        }
    }

    _ui.clearScreen();
    _ui.displayWinnerMessage(winner);
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

    while (!isSelected) {
        _ui.menuDoRowActive("menu", rowCounter);
        _ui.displayMenu();

        c = _ui.getSymbol();

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
            return SIGNAL_PREPARE_TO_GAME;
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

        c = _ui.getSymbol();

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

                    c = _ui.getSymbol();

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
                        std::string temp = OPTIONS_DESCRIPTION[rowCounter];
                        temp = temp.substr(temp.find(':') + 1);
                        if (temp == "width") {
                            _rules.setWidthField(ratio);
                        } else if (temp == "height") {
                            _rules.setHeightField(ratio);
                        }
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

void Game::loadFromFile() {
    OPTIONS_RATIOS[3] = std::to_string(_rules.getWidthField());
    OPTIONS_RATIOS[4] = std::to_string(_rules.getHeightField());
}

void Game::launcher() {
    bool isExit = false;
    short chapter = SIGNAL_MENU;
    signal(SIGINT, emergencyInterruption);

    while (!isExit) {
        _ui.clearScreen();
        switch (chapter) {
            case SIGNAL_MENU:
                chapter = openMenu();
                break;
            case SIGNAL_PREPARE_TO_GAME:
                chapter = prepareToGame();
                break;
            case SIGNAL_START_GAME:
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
