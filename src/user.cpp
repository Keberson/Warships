#include <ctime>
#include <sstream>

#include "gamerules.h"
#include "standards.h"
#include "user.h"

unsigned prevX = 0;
unsigned prevY = 0;

User::User(std::string name, unsigned widthField, unsigned heightField) {
    _name = name;
    _field = Field(widthField, heightField);
}

void User::placeShip(unsigned id , GameRules& rules) {
    // TODO(keberson): Задание для 2ой версии: установка кораблей на игровое поле либо рандом, либо считывая с консоли (сделать)
    prevX = 0;
    prevY = 0;
    Ship* currentShip = rules.getShip(id);
    int randomX;
    int randomY;
    int currentWidth = currentShip->getWidth();
    int currentLength = currentShip->getLength();
    bool isRotateAxises;
    bool isCorrectCell = false;
    while (!isCorrectCell) {
        randomX = rand() % rules.getWidthField();
        randomY = rand() % rules.getHeightField();
        isRotateAxises = rand() % 2 == 1;
        if (isRotateAxises) {
            currentWidth = currentShip->getLength();
            currentLength = currentShip->getWidth();
        }

        if (_field.getCell(randomX, randomY).getID() == 0) {
            if ((randomX + currentLength <= rules.getWidthField() - 1) &&
                (randomY + currentWidth <= rules.getHeightField() - 1)) {
                bool isFindCells = true;
                for (int offsetY = -1; offsetY <= currentWidth; ++offsetY) {
                    for (int offsetX = -1; offsetX <= currentLength; ++offsetX) {
                        if (randomX + offsetX > rules.getWidthField() - 1 || randomX + offsetX < 0 ||
                            randomY + offsetY > rules.getHeightField() - 1 || randomY + offsetY < 0) {
                            continue;
                        }

                       if (_field.getCell(randomX + offsetX, randomY + offsetY).getID() != 0) {
                            isFindCells = false;
                            break;
                        }
                    }

                    if (!isFindCells) {
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
}

bool User::attackEnemy(unsigned x, unsigned y, Field& enemyField) {
    unsigned id = enemyField.getCell(x, y).getID();
    if (id == 0) {
        enemyField.setID(x, y, 1);
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
        enemyField.setID(x, y, id + ID_SHIPS_OFFSET);
        return true;
    }

    return false;
}


short Player::turn(Field& enemyField, ConsoleUI& ui) {
    int x = prevX;
    int y = prevY;
    std::string tempStr = (char)(y + 'a') + ((x == 9) ? "10" : std::to_string(x + 1) + ' ');
    std::cout << "\033[2K" << "Now your turn..." << std::endl;
    std::cout << "Attack this cell: " << "\033[s" << tempStr << std::endl << std::endl;
    std::cout << "\033[32m" << "Use the arrows (← ↑ ↓ →) to move around the playing field, press "
                << "Enter when you select the desired cell" << "\033[37m";

    unsigned cellID = enemyField.getCell(x, y).getID();
    enemyField.setID(x, y, 2);

    char c;
    bool isEnterPressed = false;
    bool isEscape = false;
    bool isArrows = false;

    while (!isEnterPressed) {
        ui.displayTheField(enemyField, 40, true);

        c = getchar();

        if (c == 'q') {
            return SIGNAL_GAME_EXIT;
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
                case 'A':
                    if (x - 1 >= 0) {
                        x--;
                    } else {
                        x = enemyField.getWidth() - 1;
                    }

                    break;
                case 'B':
                    if (x + 1 < enemyField.getWidth()) {
                        x++;
                    } else {
                        x = 0;
                    }

                    break;
                case 'C':
                    if (y + 1 < enemyField.getHeight()) {
                        y++;
                    } else {
                        y = 0;
                    }

                    break;
                case 'D':
                    if (y - 1 >= 0) {
                        y--;
                    } else {
                        y = enemyField.getHeight() - 1;
                    }

                    break;
            }

            cellID = enemyField.getCell(x, y).getID();
            enemyField.setID(x, y, 2);
            std::string tempStr = (char)(y + 'a') + ((x == 9) ? "10" : std::to_string(x + 1) + ' ');
            std::cout << "\033[u" << tempStr;
        }

        if (c == '\n') {
            bool isInShip = false;
            for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
                if (cellID / 10 == i) {
                    isInShip = true;
                    break;
                }
            }

            if (isInShip || cellID == 0) {
                isEnterPressed = true;
                enemyField.getCell(x, y).setID(cellID);
            } else {
                ui.setCursor(24, 1);
                std::cout << "\033[2K";
                std::cout << "\033[36m" << "Try again" << "\033[37m" ;
            }
        }
    }

    ui.setCursor(24, 1);
    std::cout << "\033[2K";

    if (attackEnemy(x, y, enemyField)) {
        prevX = x;
        prevY = y;
        bool isHaveShip = false;
        if (x + 1 < enemyField.getWidth()) {
            cellID = enemyField.getCell(x + 1, y).getID();
            bool isInShip = false;
            for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
                if (cellID / 10 == i) {
                    isInShip = true;
                    break;
                }
            }

            if (isInShip) {
                std::cout << "\033[31m" << "Hit!" << "\033[37m" << std::endl;
                isHaveShip = true;
            }
        }

        if (!isHaveShip && (x - 1 < enemyField.getWidth())) {
            cellID = enemyField.getCell(x - 1, y).getID();
            bool isInShip = false;
            for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
                if (cellID / 10 == i) {
                    isInShip = true;
                    break;
                }
            }

            if (isInShip) {
                std::cout << "\033[31m" << "Hit!" << "\033[37m" << std::endl;
                isHaveShip = true;
            }
        }

        if (!isHaveShip && (y + 1 < enemyField.getWidth())) {
            cellID = enemyField.getCell(x, y + 1).getID();
            bool isInShip = false;
            for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
                if (cellID / 10 == i) {
                    isInShip = true;
                    break;
                }
            }

            if (isInShip) {
                std::cout << "\033[31m" << "Hit!" << "\033[37m" << std::endl;
                isHaveShip = true;
            }
        }

        if (!isHaveShip && (y - 1 < enemyField.getWidth())) {
            cellID = enemyField.getCell(x, y - 1).getID();
            bool isInShip = false;
            for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
                if (cellID / 10 == i) {
                    isInShip = true;
                    break;
                }
            }

            if (isInShip) {
                std::cout << "\033[31m" << "Hit!" << "\033[37m" << std::endl;
                isHaveShip = true;
            }
        }

        if (!isHaveShip) {
            // TODO(keberson): Задание для 2ой версии: обводка всех полей вокруг корабля
            // Только для кораблей в ширину 1. Переделать!!!
            /*int directionX = 0;
            int directionY = 0;
            bool isNotEdgeX = false;
            bool isNotEdgeY = false;
            bool isOneCellShip = false;

            if (x + 1 < enemyField.getHeight()) {
                cellID = enemyField.getCell(x + 1, y).getID();
                for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
                    if (cellID / 10 == i) {
                        directionX++;
                        break;
                    }
                }
            }

            if (x - 1 >= 0) {
                cellID = enemyField.getCell(x - 1, y).getID();
                for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
                    if (cellID / 10 == i) {
                        directionX++;
                        break;
                    }
                }
            }

            if (y + 1 < enemyField.getWidth()) {
                cellID = enemyField.getCell(x, y + 1).getID();
                for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
                    if (cellID / 10 == i) {
                        directionY++;
                        break;
                    }
                }
            }

            if (y - 1 >= 0) {
                cellID = enemyField.getCell(x, y - 1).getID();
                for (int i = 1; i <= ID_SHIPS_OFFSET / 10; ++i) {
                    if (cellID / 10 == i) {
                        directionY++;
                        break;
                    }
                }
            }

            if (directionX == 0 && directionY == 0) {
                isOneCellShip = true;
            }

            if (directionX == 2) {
                isNotEdgeX = true;
            }

            if (directionY == 2) {
                isNotEdgeY = true;
            }

            if (isOneCellShip) {
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        if (i != 0 || j != 0) {
                            if (x + i >= 0 && x + i < enemyField.getHeight() && y + j >= 0 &&
                                y + j < enemyField.getWidth()) {
                                enemyField.setID(x + i, y + j, 1);
                            }
                        }
                    }
                }
            }

            if (isNotEdgeX) */
            std::cout << "\033[31m" << "Destroyed!" << "\033[37m" << std::endl;
        }

        return SIGNAL_TURN_HIT;
    } else {
        prevX = x;
        prevY = y;
        std::cout << "\033[33m" << "Miss..." << "\033[37m" << std::endl;
        return SIGNAL_TURN_MISS;
    }
}


short Computer::turn(Field& enemyField, ConsoleUI& ui) {
    unsigned randomX;
    unsigned randomY;
    bool isCorrectCell = false;
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

        if (cellId == 0 || isInShip) {
            isCorrectCell = true;
        }
    }

    return attackEnemy(randomX, randomY, enemyField);
}