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
    // TODO(keberson): установка кораблей на игровое поле либо рандом, либо считывая с консоли (сделать)
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
                for (int i = -1; i <= currentWidth; ++i) {
                    for (int j = -1; j <= currentLength; ++j) {
                        int offsetX = j;
                        int offsetY = i;
                        /*if (isRotateAxises) {
                            offsetX = i;
                            offsetY = j;
                        }*/

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

    for (int i = 0; i < currentWidth; ++i) {
        for (int j = 0; j < currentLength; ++j) {
            int offsetX = j;
            int offsetY = i;
            /*if (isRotateAxises) {
                offsetX = i;
                offsetY = j;
            }*/

            _field[0].setID(randomX + offsetX, randomY + offsetY, id);
        }
    }
}

bool User::attackEnemy(unsigned x, unsigned y, Field& enemyField, unsigned offset) {
    unsigned id = enemyField.getCell(x, y).getID();
    if (id == 0) {
        enemyField.setID(x, y, 1);
        _field[1].setID(x, y, 1);
        return false;
    } else if (id / 10 == 2) {
        enemyField.setID(x, y, id + offset);
        _field[1].setID(x, y, id + offset);
        return true;
    } else {
        return false;
    }
}


bool Player::turn(Field& enemyField, unsigned numberOfShips) {
    unsigned x;
    unsigned y;
    std::string cell;
    std::cout << "Input attacking cell: ";
    std::cin >> cell;
    while (true) {
        if (cell.length() >= 3) {
            if (cell[0] >= 'a' && cell[0] <= 'j' && cell.substr(1) == "10") {
                y = cell[0] - 'a';
                x = 9;
                if (enemyField.getCell(x, y).getID() == 0 || enemyField.getCell(x, y).getID() / 10 == 2) {
                    break;
                }
            }
        } else {
            if (cell[0] >= 'a' && cell[0] <= 'j' && cell[1] >= '1' && cell[1] <= '9') {
                y = cell[0] - 'a';
                x = cell[1] - '1';
                if (enemyField.getCell(x, y).getID() == 0 || enemyField.getCell(x, y).getID() / 10 == 2) {
                    break;
                }
            }
        }

        std::cout << "Reinput attacking cell: ";
        std::cin >> cell;
    }

    return attackEnemy(x, y, enemyField, numberOfShips);
}


bool Computer::turn(Field& enemyField, unsigned numberOfShips) {
    unsigned randomX;
    unsigned randomY;
    bool isCorrectCell = false;
    while (!isCorrectCell) {
        randomX = rand() % 10;
        randomY = rand() % 10;
        if (enemyField.getCell(randomX, randomY).getID() == 0 || enemyField.getCell(randomX, randomY).getID() / 10 == 2) {              // TODO(keberson): изменить условие
            isCorrectCell = true;
        }
    }

    unsigned offset = numberOfShips + numberOfShips % 10;
    return attackEnemy(randomX, randomY, enemyField, offset);
}