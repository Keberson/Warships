#ifndef WARSHIPS_STANDARDS_H
#define WARSHIPS_STANDARDS_H

#include <string>

const std::string STANDARD_CONFIG_PATH = "configs/game.cg";
const unsigned STANDARD_ID_START = 20;

// chapter "rules"
#define STANDARD_FIELD_WIDTH 10
#define STANDARD_FIELD_HEIGHT 10
#define STANDARD_NUMBER_OF_SHIPS 10
#define STANDARD_CUSTOM_SHIPS false
#define STANDARD_TURN_TIME 30
#define STANDARD_BONUSES false

const std::string STANDARD_SHIPS_NAME[] = { "OneCellShip", "TwoCellShip", "ThreeCellShip", "FourCellShip" };
const unsigned STANDARD_SHIPS_WIDTH[] = { 1, 2, 3, 4 };
const unsigned STANDARD_SHIPS_HEIGHT[] = { 1, 1, 1, 1};
const unsigned STANDARD_SHIPS_ATTACK_RADIUS[] = { 1, 1, 1, 1 };
const unsigned STANDARD_SHIPS_NUMBER[] = { 4, 3, 2, 1 };

#endif // WARSHIPS_STANDARDS_H
