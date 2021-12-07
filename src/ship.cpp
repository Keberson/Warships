#include "ship.h"

void Ship::attackShip(unsigned int damage) {
    _hp -= damage;
}