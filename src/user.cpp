#include <ctime>

#include "gamerules.h"
#include "standards.h"
#include "user.h"

// User
User::User(std::string name, unsigned widthField, unsigned heightField) {
    _name = name;
    std::vector<Field> tempVector = { Field(widthField, heightField), Field(widthField, heightField) };
    _field = tempVector;
}

void User::placeShip(unsigned id , GameRules& rules) {
    // TODO(keberson): установка кораблей на игровое поле либо рандом, либо считывая с консоли
    Ship* currentShip = rules.getShip(id);
    int randomX;
    int randomY;
    bool isCorrectCell = false;
    while (!isCorrectCell) {
        randomX = rand() % rules.getWidthField();
        randomY = rand() % rules.getHeightField();
        if (_field[0].getCell(randomX, randomY).getID() == 0) {

            if ((randomX + currentShip->getLength() - 1 <= rules.getWidthField() - 1) && (randomY + currentShip->getWidth() - 1 <= rules.getHeightField() - 1)) {
                bool isFindCells = true;
                for (int i = -1; i <= (int)currentShip->getWidth(); ++i) {
                    for (int j = -1; j <= (int)currentShip->getLength(); ++j) {
                        if (randomX + j > rules.getWidthField() - 1 || randomX + j < 0 ||
                            randomY + i > rules.getHeightField() - 1 || randomY + i < 0) {
                            continue;
                        }

                       if (_field[0].getCell(randomX + j, randomY + i).getID() != 0) {
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

    for (int width = 0; width < currentShip->getWidth(); ++width) {
        for (int length = 0; length < currentShip->getLength(); ++length) {
            _field[0].setID(randomX + length, randomY + width, id);
        }
    }
}

void User::attackEnemy(unsigned x, unsigned y, Field& enemyField, unsigned offset) {
    unsigned id = enemyField.getCell(x, y).getID();
    if (id == 0) {
        enemyField.setID(x, y, 1);
        _field[1].setID(x, y, 1);
    } else {
        enemyField.setID(x, y, id + offset);
        _field[1].setID(x, y, id + offset);
    }
}

// /User

// Player

void Player::turn(Field& enemyField, unsigned numberOfShips) {
    unsigned x;
    unsigned y;
    std::string cell;
    std::cout << "Input attacking cell: ";
    std::cin >> cell;
    while (true) {
        if (cell.length() == 3) {
            if (cell[0] >= 'a' && cell[0] <= 'k'&& cell.substr(1) == "10") {
                x = cell[0] - 'a';
                y = 9;
                break;
            }
        } else {
            if (cell[0] >= 'a' && cell[0] <= 'k' && cell[1] >= '1' && cell[1] <= '9') {
                x = cell[0] - 'a';
                y = cell[1] - '1';
                break;
            }
        }

        std::cout << "Reinput attacking cell: ";
        std::cin >> cell;
    }

    attackEnemy(x, y, enemyField, numberOfShips);
}

// /Player

// Computer

void Computer::turn(Field& enemyField, unsigned numberOfShips) {
    unsigned randomX;
    unsigned randomY;
    bool isCorrectCell = false;
    while (!isCorrectCell) {
        randomX = rand() % 10;
        randomY = rand() % 10;
        if (enemyField.getCell(randomX, randomY).getID() == 0) {
            isCorrectCell = true;
        }
    }

    unsigned offset = numberOfShips + numberOfShips % 10;
    attackEnemy(randomX, randomY, enemyField, offset);
}

// /Computer
