#include <ctime>

#include "gamerules.h"
#include "standards.h"
#include "user.h"

User::User(std::string name, size_t widthField, size_t heightField) {
    _name = name;
    std::vector<Field> tempVector = { Field(widthField, heightField), Field(widthField, heightField) };
    _field = tempVector;
}

void User::placeShip(size_t id , GameRules& rules) {
    // TODO(keberson): установка кораблей на игровое поле либо рандом, либо считывая с консоли (сделать)
    Ship* currentShip = rules.getShip(id);
    int randomX;
    int randomY;
    bool isInvertAxises;
    bool isCorrectCell = false;
    while (!isCorrectCell) {
        randomX = rand() % rules.getWidthField();
        randomY = rand() % rules.getHeightField();
        isInvertAxises = rand() % 2 == 1;
        if (_field[0].getCell(randomX, randomY).getID() == 0) {
            if ((randomX + ((int)currentShip->getLength() - 1) <= rules.getWidthField() - 1) &&
                (randomY + ((int)currentShip->getWidth() - 1) <= rules.getHeightField() - 1)) {
                bool isFindCells = true;
                for (int i = -1; i <= (int)currentShip->getWidth(); ++i) {
                    for (int j = -1; j <= (int)currentShip->getLength(); ++j) {
                        int offsetX = j;
                        int offsetY = i;
                        if (isInvertAxises) {
                            offsetX = i;
                            offsetY = j;
                        }

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

    for (int i = 0; i < currentShip->getWidth(); ++i) {
        for (int j = 0; j < currentShip->getLength(); ++j) {
            int offsetX = j;
            int offsetY = i;
            if (isInvertAxises) {
                offsetX = i;
                offsetY = j;
            }

            _field[0].setID(randomX + offsetX, randomY + offsetY, id);
        }
    }
}

bool User::attackEnemy(size_t x, size_t y, Field& enemyField, size_t offset) {
    size_t id = enemyField.getCell(x, y).getID();
    if (id == 0) {
        enemyField.setID(x, y, 1);
        _field[1].setID(x, y, 1);
        return false;
    } else {
        enemyField.setID(x, y, id + offset);
        _field[1].setID(x, y, id + offset);
        return true;
    }
}


bool Player::turn(Field& enemyField, size_t numberOfShips) {
    size_t x;
    size_t y;
    std::string cell;
    std::cout << "Input attacking cell: ";
    std::cin >> cell;
    while (true) {
        if (cell.length() >= 3) {
            if (cell[0] >= 'a' && cell[0] <= 'j' && cell.substr(1) == "10") {
                y = cell[0] - 'a';
                x = 9;
                break;
            }
        } else {
            if (cell[0] >= 'a' && cell[0] <= 'j' && cell[1] >= '1' && cell[1] <= '9') {
                y = cell[0] - 'a';
                x = cell[1] - '1';
                break;
            }
        }

        std::cout << "Reinput attacking cell: ";
        std::cin >> cell;
    }

    return attackEnemy(x, y, enemyField, numberOfShips);
}


bool Computer::turn(Field& enemyField, size_t numberOfShips) {
    size_t randomX;
    size_t randomY;
    bool isCorrectCell = false;
    while (!isCorrectCell) {
        randomX = rand() % 10;
        randomY = rand() % 10;
        if (enemyField.getCell(randomX, randomY).getID() == 0) {
            isCorrectCell = true;
        }
    }

    size_t offset = numberOfShips + numberOfShips % 10;
    return attackEnemy(randomX, randomY, enemyField, offset);
}