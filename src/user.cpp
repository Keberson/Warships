#include <ctime>

#include "gamerules.h"
#include "standards.h"
#include "user.h"

User::User(std::string name, unsigned widthField, unsigned heightField) {
    _name = name;
    std::vector<Field> tempVector = { Field(widthField, heightField), Field(widthField, heightField) };
    _field = tempVector;
}

void User::placeShip(unsigned id , GameRules& rules) {
    // TODO(keberson): Задание для 2ой версии: установка кораблей на игровое поле либо рандом, либо считывая с консоли (сделать)
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

        if (_field[0].getCell(randomX, randomY).getID() == 0) {
            if ((randomX + currentLength <= rules.getWidthField() - 1) &&
                (randomY + currentWidth <= rules.getHeightField() - 1)) {
                bool isFindCells = true;
                for (int offsetY = -1; offsetY <= currentWidth; ++offsetY) {
                    for (int offsetX = -1; offsetX <= currentLength; ++offsetX) {
                        if (randomX + offsetX > rules.getWidthField() - 1 || randomX + offsetX < 0 ||
                            randomY + offsetY > rules.getHeightField() - 1 || randomY + offsetY < 0) {
                            continue;
                        }

                       if (_field[0].getCell(randomX + offsetX, randomY + offsetY).getID() != 0) {
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
            _field[0].setID(randomX + offsetX, randomY + offsetY, id);
        }
    }
}

bool User::attackEnemy(unsigned x, unsigned y, Field& enemyField) {
    unsigned id = enemyField.getCell(x, y).getID();
    if (id == 0) {
        enemyField.setID(x, y, 1);
        _field[1].setID(x, y, 1);
        return false;
    } else if (id / 10 == 1) {
        enemyField.setID(x, y, id + ID_SHIPS_OFFSET);
        _field[1].setID(x, y, id + ID_SHIPS_OFFSET);
        return true;
    } else {
        return false;
    }
}


bool Player::turn(Field& enemyField) {
    unsigned x;
    unsigned y;
    std::string cell;
    std::cout << "\033[2K" << "Input attacking cell: ";
    std::cin >> cell;                           // TODO(danza): Задание для 2ой версии: изменить систему ввода данных
    if (std::cin.eof()) {
        std::cout << "\033[?25h" << std::endl;
        exit(2);
    }

    while (true) {
        if (cell.length() >= 3) {
            if (cell[0] >= 'a' && cell[0] <= 'j' && cell.substr(1) == "10") {
                y = cell[0] - 'a';
                x = 9;
                if (enemyField.getCell(x, y).getID() == 0 || enemyField.getCell(x, y).getID() / 10 == 1) {
                    break;
                }
            }
        } else {
            if (cell[0] >= 'a' && cell[0] <= 'j' && cell[1] >= '1' && cell[1] <= '9') {
                y = cell[0] - 'a';
                x = cell[1] - '1';
                if (enemyField.getCell(x, y).getID() == 0 || enemyField.getCell(x, y).getID() / 10 == 1) {
                    break;
                }
            }
        }

        std::cout << "\033A" << "\033[2K";
        std::cout << "Reinput attacking cell: ";
        std::cin >> cell;
        if (std::cin.eof()) {
            std::cout << "\033[?25h" << std::endl;
            exit(2);
        }
    }

    std::cout << "\033A" << "\033[2K";

    if (attackEnemy(x, y, enemyField)) {
        bool isHaveShip = false;
        if (x + 1 < enemyField.getWidth()) {
            if (enemyField.getCell(x + 1, y).getID() / 10 == 1) {
                std::cout << "Hit!" << std::endl;
                isHaveShip = true;
            }
        }

        if (!isHaveShip && (x - 1 < enemyField.getWidth())) {
            if (enemyField.getCell(x - 1, y).getID() / 10 == 1) {
                std::cout << "Hit!" << std::endl;
                isHaveShip = true;
            }
        }

        if (!isHaveShip && (y + 1 < enemyField.getWidth())) {
            if (enemyField.getCell(x, y + 1).getID() / 10 == 1) {
                std::cout << "Hit!" << std::endl;
                isHaveShip = true;
            }
        }
        if (!isHaveShip && (y -1 < enemyField.getWidth())) {
            if (enemyField.getCell(x, y - 1).getID() / 10 == 1) {
                std::cout << "Hit!" << std::endl;
                isHaveShip = true;
            }
        }

        if (!isHaveShip) {
            // TODO(danza_): Задание для 2ой версии: обводка всех полей вокруг корабля
            std::cout << "Destroyed!" << std::endl;
        }

        return true;
    } else {
        std::cout << "Miss..." << std::endl;
        return false;
    }
}


bool Computer::turn(Field& enemyField) {
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