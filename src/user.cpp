#include "gamerules.h"
#include "standards.h"
#include "user.h"

// User
User::User(std::string name, unsigned widthField, unsigned heightField) {
    _name = name;
    std::vector<Field> tempVector = { Field(widthField, heightField), Field(widthField, heightField) };
    _field = tempVector;
}

void User::changeCellState(unsigned x, unsigned y, unsigned id) {
    // TODO(keberson): Переключение между полями атаки и защиты
    _field[0].setID(x, y, id);
}

void User::placeShip(unsigned id , GameRules& rules) {
    // TODO(keberson): установка кораблей на игровое поле
    unsigned randomX;
    int randomDirection;
    unsigned randomY;
    bool isCorrectCell = false;
    while (!isCorrectCell) {
        randomX = rand() % rules.getWidthField();
        randomDirection = (rand() % 2 == 0) ? -1 : +1 ;
        randomY = rand() % rules.getHeightField();
        if (randomX + randomDirection*rules.getShip(id)->getWidth() < rules.getWidthField() &&
        randomY + randomDirection*rules.getShip(id)->getLength() < rules.getHeightField()) {
            continue;
        }

        if (_field[0].getCell(randomX, randomY).getID() == 0) {
            isCorrectCell = true;
            break;
        }

        bool isFindAnyID = false;
        for (unsigned length = 0; length < rules.getShip(id)->getLength(); ++length) {
            for (unsigned width = 0; width < rules.getShip(id)->getWidth(); ++width) {
                if (_field[0].getCell(randomX + randomDirection*length, randomY + randomDirection*width).getID() != 0) {
                    isFindAnyID = true;
                    break;
                }
            }

            if (isFindAnyID) {
                break;
            }
        }

        if (!isFindAnyID) {
            isCorrectCell = true;
        }
    }

    for (unsigned length = 0; length < rules.getShip(id)->getLength(); ++length) {
        for (unsigned width = 0; width < rules.getShip(id)->getWidth(); ++width) {
            _field[0].setID(randomY + randomDirection*length, randomY + randomDirection*width, id);
        }
    }
}

// /User

// Player

int Player::turn() {
    return 0;
}

// /Player

// Computer

int Computer::turn() {
    return 0;
}

// /Computer
