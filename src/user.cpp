#include <ctime>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include "gamerules.h"
#include "standards.h"
#include "user.h"

unsigned prevX = 0;
unsigned prevY = 0;

User::User(std::string name, unsigned widthField, unsigned heightField, std::vector<Ship> ships) {
    _name = name;
    _field = Field(widthField, heightField, ships);
}

void User::randomPlaceShip(unsigned id) {
    prevX = 0;
    prevY = 0;
    Ship* currentShip = _field.getShip(id);
    int randomX;
    int randomY;
    int currentWidth = currentShip->getWidth();
    int currentLength = currentShip->getLength();
    bool isRotateAxises;
    bool isCorrectCell = false;
    std::vector<Cell*> border;
    while (!isCorrectCell) {
        randomX = rand() % _field.getWidth();
        randomY = rand() % _field.getHeight();
        isRotateAxises = rand() % 3 != 0;
        if (isRotateAxises) {
            currentWidth = currentShip->getLength();
            currentLength = currentShip->getWidth();
        }

        if (_field.getCell(randomX, randomY).getID() == EMPTY_CELL_ID) {
            if ((randomX + currentLength <= _field.getWidth() - 1) &&
                (randomY + currentWidth <= _field.getHeight() - 1)) {
                bool isFindCells = true;
                for (int offsetY = -1; offsetY <= currentWidth; ++offsetY) {
                    for (int offsetX = -1; offsetX <= currentLength; ++offsetX) {
                        if (randomX + offsetX > _field.getWidth() - 1 || randomX + offsetX < 0 ||
                            randomY + offsetY > _field.getHeight() - 1 || randomY + offsetY < 0) {
                            continue;
                        }

                       if (_field.getCell(randomX + offsetX, randomY + offsetY).getID() / 10 == 1) {
                            isFindCells = false;
                            break;
                        }

                       if (offsetY == -1 || offsetY == currentWidth) {
                           border.push_back(&_field.getCell(randomX + offsetX, randomY + offsetY));
                       } else if (offsetX == -1 || offsetX == currentLength) {
                           border.push_back(&_field.getCell(randomX + offsetX, randomY + offsetY));
                       } else if (_field.getCell(randomX + offsetX, randomY + offsetY).getID() == ISLAND_ID) {
                           isFindCells = false;
                           break;
                       }
                    }

                    if (!isFindCells) {
                        border.clear();
                        break;
                    }
                }

                if (isFindCells) {
                    isCorrectCell = true;
                }
            }
        }

    }

    for (unsigned offsetY = 0; offsetY < currentWidth; ++offsetY) {
        for (unsigned offsetX = 0; offsetX < currentLength; ++offsetX) {
            _field.setID(randomX + offsetX, randomY + offsetY, id);
        }
    }

    _field.addShipBorder(id, border);
}

void User::randomPlaceIsland(unsigned number) {
    for (int i = 0; i < number; ++i) {
        bool isCorrectCell = false;
        while (!isCorrectCell) {
            unsigned randomX = rand() % _field.getWidth();
            unsigned randomY = rand() % _field.getHeight();

            if (_field.getCell(randomX, randomY).getID() == EMPTY_CELL_ID) {
                _field.setID(randomX, randomY, ISLAND_ID);
                isCorrectCell = true;
            }
        }
    }
}

bool User::attackEnemy(unsigned x, unsigned y, Field& enemyField) {
    unsigned id = enemyField.getCell(x, y).getID();
    if (id == EMPTY_CELL_ID) {
        enemyField.setID(x, y, 1);
        return false;
    }

    if (id == ISLAND_ID) {
        enemyField.setID(x, y, ISLAND_ID_ATTACKED);
        return false;
    }

    bool isInShip = false;
    for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
        if (id / 10 == i) {
            isInShip = true;
            break;
        }
    }

    if (isInShip) {
        enemyField.getShip(id)->attackShip(1);
        enemyField.setNumberOfHits(1);
        enemyField.setID(x, y, id + ID_SHIPS_OFFSET);
        return true;
    }

    return false;
}

short User::saveSession(std::string name, Field& enemyField) {
    std::fstream file;
    file.open(name, std::ios_base::out);
    if (!file.is_open()) {
        return ERROR_OPEN;
    }

    for (int i = 0; i < 2; ++i) {
        std::string title = "field:";
        if (i == 1) {
            title = "enemyField:";
        }

        file << title << std::endl;
        file << '\t' << "width:" << _field.getWidth() << std::endl;
        file << '\t' << "height:" << _field.getHeight() << std::endl;

        std::vector<Cell> cells = _field.getAllCells();
        std::vector<Cell> enemyCells = enemyField.getAllCells();

        for (int j = 0; j < cells.size(); ++j) {
            if (i == 1) {
                if (enemyCells[j].getID() == 0) {
                    continue;
                }
            } else {
                if (cells[j].getID() == 0) {
                    continue;
                }
            }

            file << '\t' << "Cell" << ':' << std::endl;
            file << "\t\t" << "x:" << ((i == 1) ? enemyCells[j].getX() : cells[j].getX()) << std::endl;
            file << "\t\t" << "y:" << ((i == 1) ? enemyCells[j].getY() : cells[j].getY()) << std::endl;
            file << "\t\t" << "id:" << ((i == 1) ? enemyCells[j].getID() : cells[j].getID()) << std::endl;
            file << '\t' << "~~~" << std::endl;
        }

        std::vector<Ship> ships = _field.getAllShips();
        std::vector<Ship> enemyShips = enemyField.getAllShips();

        for (int j = 0; j < ships.size(); ++j) {
            file << '\t' << "Ship" << ":" << std::endl;
            file << "\t\t" << "id:" << ((i == 1) ? enemyShips[j].getId() : ships[j].getId()) << std::endl;
            file << "\t\t" << "name:" << ((i == 1) ? enemyShips[j].getName() : ships[j].getName()) << std::endl;
            file << "\t\t" << "width:" << ((i == 1) ? enemyShips[j].getWidth() : ships[j].getWidth()) << std::endl;
            file << "\t\t" << "length:" << ((i == 1) ? enemyShips[j].getLength() : ships[j].getLength()) << std::endl;
            file << '\t' << "~~~" << std::endl;
        }

        for (int j = 0; j < ships.size(); ++j) {
            file  << "\tBorder" << ":" << std::endl;
            file << "\t\t" << "id:" << ((i == 1) ? enemyShips[j].getId() : ships[j].getId()) << std::endl;
            std::vector<Cell*> shipBorder;
            if (i == 1) {
                shipBorder = enemyField.getBorderCells(enemyShips[j].getId());
            } else {
                shipBorder = _field.getBorderCells(ships[j].getId());
            }

            for (auto item: shipBorder) {
                file << "\t\t" << "Cell" << ":" << std::endl;
                file << "\t\t\t" << "x:" << item->getX() << std::endl;
                file << "\t\t\t" << "y:" << item->getY() << std::endl;
                file << "\t\t\t" << "id:" << item->getID() << std::endl;
                file << "\t\t" << "~~~" << std::endl;
            }

            file << "\t" << "~~~" << std::endl;

        }

        file << "~~~" << std::endl;
    }

    file.close();
    return SUCCESS;
}

short User::loadSession(std::string name, Field& enemyField) {
    std::fstream file(name);
    if (!file.is_open()) {
        return ERROR_OPEN;
    }

    std::string line;
    bool isField = false;
    bool isEnemyField = false;
    bool isCell = false;
    bool isShip = false;
    bool isBorder = false;
    bool isHaveWidth = false;
    bool isHaveHeight = false;
    bool isHaveX = false;
    bool isHaveY = false;
    bool isHaveID = false;
    bool isHaveShipID = false;
    bool isHaveShipName = false;
    bool isHaveShipWidth = false;
    bool isHaveShipLength = false;
    bool isHaveShipBorderID = false;
    unsigned width;
    unsigned height;
    unsigned x;
    unsigned y;
    unsigned id;
    unsigned idShip;
    std::string nameShip;
    unsigned widthShip;
    unsigned lengthShip;
    unsigned idShipBorder;
    std::vector<Cell> notZerosCells;
    std::vector<Ship> ships;
    std::vector<Cell> borderShips;
    std::vector<std::vector<Cell>> borders;
    std::vector<unsigned> idBorders;
    int i = 0;
    while (getline(file, line)) {
        i++;
        if (isField) {
            auto commentPos = line.find("//");
            if (commentPos != std::string::npos) {
                line = line.substr(0, commentPos);
            }

            auto symbolPos = line.find(':');
            if (symbolPos != std::string::npos) {
                auto beforeSymbol = line.substr(0, symbolPos);
                auto afterSymbol = line.substr(symbolPos+1);
                beforeSymbol = deleteSymbols(beforeSymbol, "\t ");
                afterSymbol = deleteSymbols(afterSymbol, "\t \n\r");
                if (afterSymbol.empty()) {
                    if (beforeSymbol == "Cell") {
                        isCell = true;
                    } else if (beforeSymbol == "Ship") {
                        isShip = true;
                    } else if (beforeSymbol == "Border") {
                        isBorder = true;
                    } else {
                        return ERROR_OPEN;
                    }
                } else {
                    if (beforeSymbol == "width" && !isShip) {
                        width = atoi(afterSymbol.c_str());
                        isHaveWidth = true;
                    } else if (beforeSymbol == "height" && !isShip) {
                        height = atoi(afterSymbol.c_str());
                        isHaveHeight = true;
                    }

                    if (isCell) {
                        if (beforeSymbol == "x") {
                            x = atoi(afterSymbol.c_str());
                            isHaveX = true;
                        } else if (beforeSymbol == "y") {
                            y = atoi(afterSymbol.c_str());
                            isHaveY = true;
                        } else if (beforeSymbol == "id") {
                            id = atoi(afterSymbol.c_str());
                            isHaveID = true;
                        }
                    } else if (isShip) {
                        if (beforeSymbol == "id") {
                            idShip = atoi(afterSymbol.c_str());
                            isHaveShipID = true;
                        } else if (beforeSymbol == "name") {
                            nameShip = afterSymbol;
                            isHaveShipName = true;
                        } else if (beforeSymbol == "width") {
                            widthShip = atoi(afterSymbol.c_str());
                            isHaveShipWidth = true;
                        } else if (beforeSymbol == "length") {
                            lengthShip = atoi(afterSymbol.c_str());
                            isHaveShipLength = true;
                        }
                    } else if (isBorder) {
                        if (beforeSymbol == "id") {
                            idShipBorder = atoi(afterSymbol.c_str());
                            isHaveShipBorderID = true;
                        }
                    }
                }
            }
        }

        if (line.find("field") != std::string::npos) {
            isField = true;
        } else if (line.find("enemyField") != std::string::npos) {
            isField = true;
            isEnemyField = true;
        } else if (line.find("~~~") != std::string::npos) {
            if (isShip) {
                if (!isHaveShipID || !isHaveShipName || !isHaveShipWidth || !isHaveShipLength) {
                    return ERROR_OPEN;
                }

                ships.push_back(Ship(idShip, nameShip, widthShip, lengthShip));

                isHaveShipID = false;
                isHaveShipName = false;
                isHaveShipWidth = false;
                isHaveShipLength = false;
                isShip = false;
            } else if (isCell) {
                if (!isHaveX || !isHaveY || !isHaveID) {
                    return ERROR_OPEN;
                }

                if (isBorder) {
                    borderShips.push_back(Cell(x, y, id));
                } else {
                    notZerosCells.push_back(Cell(x, y, id));
                }


                isHaveX = false;
                isHaveY = false;
                isHaveID = false;
                isCell = false;
            } else if (isBorder) {
                if (!isHaveShipBorderID || borderShips.size() == 0) {
                    return ERROR_OPEN;
                }

                borders.push_back(borderShips);
                idBorders.push_back(idShipBorder);

                borderShips.clear();
                isHaveShipID = false;
                isBorder = false;
            } else if (isField) {
                if (!isHaveWidth || !isHaveHeight) {
                    return ERROR_OPEN;
                }

                Field temp(width, height, ships);
                for (auto item: notZerosCells) {
                    temp.setID(item.getX(), item.getY(), item.getID());
                }

                if (isEnemyField) {
                    enemyField = temp;
                    isEnemyField = false;
                    for (int i = 0; i < borders.size(); ++i) {
                        std::vector<Cell*> temp;
                        for (auto item: borders[i]) {
                            temp.push_back(&enemyField.getCell(item.getX(), item.getY()));
                        }

                        enemyField.addShipBorder(idBorders[i], temp);
                    }
                } else {
                    _field = temp;
                    for (int i = 0; i < borders.size(); ++i) {
                        std::vector<Cell*> temp;
                        for (auto item: borders[i]) {
                            temp.push_back(&_field.getCell(item.getX(), item.getY()));
                        }

                        _field.addShipBorder(idBorders[i], temp);
                    }
                }

                ships.clear();
                notZerosCells.clear();
                borders.clear();
                idBorders.clear();
                isField = false;
            }
        }
    }

    return SUCCESS;
}

short Player::turn(Field& enemyField, ConsoleUI& ui) {
    int x = prevX;
    int y = prevY;

    unsigned cellID = enemyField.getCell(x, y).getID();
    enemyField.setID(x, y, 2);

    char c;
    bool isEnterPressed = false;
    bool isEscape = false;
    bool isArrows = false;

    std::string options = "player ";
    options += "cell:";
    options += (char)(y + 'a');
    options += std::to_string(x + 1) + ';';
    options += "isFirstTurn;";
    ui.displayTheField(enemyField, "right", true, options);

    while (!isEnterPressed) {
        c = ui.getSymbol();

        if (c == 'q') {
            return SIGNAL_GAME_EXIT;
        }

        if (c == 's') {
            bool isExit = false;
            options.clear();
            options += "player";
            options += "cell:";
            options += (char)(y + 'a');
            options += std::to_string(x + 1) + ';';
            options += "isSaveLoad:";
            ui.displayTheField(enemyField, "right", true, options);
            bool isInputEnd = false;
            std::string filename;
            while (!isInputEnd) {
                c = getchar();

                if (c == 27) {
                    isExit = true;
                    break;
                } else if (c == '\n') {
                    isInputEnd = true;
                } else if (c == 127) {
                    if (filename.size() >= 1) {
                        filename.pop_back();
                        options.pop_back();
                    }
                } else if ((c >= '0' && c <= '9') || (tolower(c) >= 'a' && tolower(c) <= 'z') || c == '.' || c == '_') {
                    filename += c;
                    options += c;
                }

                ui.displayTheField(enemyField, "right", true, options);
            }

            c = '\0';
            if (!isExit) {
                enemyField.setID(x, y, cellID);
                options.clear();
                std::string temp;
                if (saveSession(filename, enemyField) != SUCCESS) {
                    temp = "Unable to open file ";
                } else {
                    temp = "Success saved to ";
                }

                enemyField.setID(x, y, 2);
                options += "player ";
                options += "isFirstTurn;";
                ui.clearScreen();
                ui.displayFields(getField(), enemyField, options);
                options = "player ";
                options += "cell:";
                options += (char)(y + 'a');
                options += std::to_string(x + 1) + ';';
                options += "isOpened" + temp + filename;
                ui.displayFields(getField(), enemyField, options);
            } else {
                std::string options;
                options = "player ";
                options += "cell:";
                options += (char)(y + 'a');
                options += std::to_string(x + 1) + ';';
                options += "isFirstTurn;";
                ui.displayTheField(enemyField, "right", true, options);
            }
        }

        if (c == 'l') {
            bool isExit = false;
            options.clear();
            options += "player";
            options += "cell:";
            options += (char)(y + 'a');
            options += std::to_string(x + 1) + ';';
            options += "isSaveLoad:";
            ui.displayTheField(enemyField, "right", true, options);
            bool isInputEnd = false;
            std::string filename;
            while (!isInputEnd) {
                c = ui.getSymbol();

                if (c == 27) {
                    isExit = true;
                    break;
                } else if (c == '\n') {
                    isInputEnd = true;
                } else if (c == 127) {
                    filename.pop_back();
                    options.pop_back();
                } else {
                    filename += c;
                    options += c;
                }

                ui.displayFields(getField(), enemyField, options);
            }

            c = '\0';
            if (!isExit) {
                enemyField.setID(x, y, cellID);
                options.clear();
                std::string temp;
                if (loadSession(filename, enemyField) != SUCCESS) {
                    temp = "Unable to open file ";
                } else {
                    prevX = 0;
                    prevY = 0;
                    x = 0;
                    y = 0;
                    temp = "Success loaded to ";
                }

                cellID = enemyField.getCell(x, y).getID();
                enemyField.setID(x, y, 2);
                options += "player ";
                options += "isFirstTurn;";
                ui.clearScreen();
                ui.displayFields(getField(), enemyField, options);
                options = "player ";
                options += "cell:";
                options += (char)(y + 'a');
                options += std::to_string(x + 1) + ';';
                options += "isOpened" + temp + filename;
                ui.displayFields(getField(), enemyField, options);
            } else {
                std::string options;
                options = "player ";
                options += "cell:";
                options += (char)(y + 'a');
                options += std::to_string(x + 1) + ';';
                options += "isFirstTurn;";
                ui.displayTheField(enemyField, "right", true, options);
            }
        }

        if (c == '\033') {
            isEscape = true;
        }

        if (isEscape && c == '[') {
            isArrows = true;
        }

        if (isArrows) {
            enemyField.setID(x, y, cellID);

            switch (c) {
                case 'D':
                    if (x - 1 >= 0) {
                        x--;
                    } else {
                        x = enemyField.getWidth() - 1;
                    }

                    break;
                case 'C':
                    if (x + 1 < enemyField.getWidth()) {
                        x++;
                    } else {
                        x = 0;
                    }

                    break;
                case 'B':
                    if (y + 1 < enemyField.getHeight()) {
                        y++;
                    } else {
                        y = 0;
                    }

                    break;
                case 'A':
                    if (y - 1 >= 0) {
                        y--;
                    } else {
                        y = enemyField.getHeight() - 1;
                    }

                    break;
            }

            cellID = enemyField.getCell(x, y).getID();
            enemyField.setID(x, y,2);
            options = "player ";
            options += "cell:";
            options += (char)(y + 'a');
            options += std::to_string(x + 1) + ';';
            ui.displayTheField(enemyField, "right", true, options);
        }

        if (c == '\n') {
            bool isInShip = false;
            for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
                if (cellID / 10 == i) {
                    isInShip = true;
                    break;
                }
            }

            if (isInShip || cellID == EMPTY_CELL_ID || cellID == ISLAND_ID) {
                isEnterPressed = true;
                enemyField.getCell(x, y).setID(cellID);
            } else {
                options = "player ";
                options += "cell:" + (char)(y + 'a') + std::to_string(x + 1) + ';';
                options += "isInvalidCell;";
                ui.displayTheField(enemyField, "right", true, options);
            }
        }
    }

    unsigned prevCellID = enemyField.getCell(x, y).getID();

    if (attackEnemy(x, y, enemyField)) {
        prevX = x;
        prevY = y;

        if (enemyField.getShip(prevCellID)->getHP() > 0) {
            options = "player ";
            options += "cell:";
            options += (char)(y + 'a');
            options += std::to_string(x + 1) + ';';
            options += "isHit;";
            ui.displayTheField(enemyField, "right", true, options);
        } else {
            std::vector<Cell*> border = enemyField.getBorderCells(prevCellID);

            for (auto item: border) {
                if (item->getID() == ISLAND_ID) {
                    item->setID(ISLAND_ID_ATTACKED);
                } else {
                    item->setID(1);
                }
            }

            options = "player ";
            options += "cell:";
            options += (char)(y + 'a');
            options += std::to_string(x + 1) + ';';
            options += "isDestroyed;";
            ui.displayTheField(enemyField, "right", true, options);
        }

        return SIGNAL_TURN_HIT;
    } else {
        prevX = x;
        prevY = y;
        options = "player ";
        options += "cell:";
        options += (char)(y + 'a');
        options += std::to_string(x + 1) + ';';
        options += "isMiss;";
        ui.displayTheField(enemyField, "right", true, options);
        return SIGNAL_TURN_MISS;
    }
}


short Computer::turn(Field& enemyField, ConsoleUI& ui) {
    unsigned randomX;
    unsigned randomY;
    bool isCorrectCell = false;
    std::string options = "computer ";

    while (!isCorrectCell) {
        randomX = rand() % enemyField.getWidth();
        randomY = rand() % enemyField.getHeight();
        unsigned cellId = enemyField.getCell(randomX, randomY).getID();
        bool isInShip = false;
        for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
            if (cellId / 10 == i) {
                isInShip = true;
                break;
            }
        }

        if (cellId == EMPTY_CELL_ID || isInShip) {
            isCorrectCell = true;
        }
    }

    options += "isFirstTurn;";
    ui.displayTheField(enemyField, "left", false, options);
    sleep(1);
    options = "computer ";

    unsigned prevCellID = enemyField.getCell(randomX, randomY).getID();
    bool isGot = false;

    if (attackEnemy(randomX, randomY, enemyField)) {
        if (enemyField.getShip(prevCellID)->getHP() <= 0) {
            std::vector<Cell*> border = enemyField.getBorderCells(prevCellID);

            for (auto item: border) {
                if (item->getID() == ISLAND_ID) {
                    item->setID(ISLAND_ID_ATTACKED);
                } else {
                    item->setID(1);
                }
            }

            options += "isDestroyed;";
        } else {
            options += "isHit;";
        }

        isGot = true;
    } else {
        options += "isMiss;";
    }

    ui.displayTheField(enemyField, "left", false, options);
    sleep(1);

    return isGot;
}