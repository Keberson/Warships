#ifndef WARSHIPS_GAMERULES_H
#define WARSHIPS_GAMERULES_H

#include <string>
#include <vector>

#include "bonus.h"
#include "field.h"
#include "parser.h"
#include "standards.h"

class GameRules {
private:
    size_t _widthField;
    size_t _heightField;
    size_t _numberOfShips;
    bool _isCustomShips;
    size_t _turnTime;
    bool _isActiveBonuses;
    size_t _squareOfShips;
    std::vector<Bonus> _bonuses;
    std::vector<Ship> _ships;
    Parser parser;
public:
    GameRules() {};
    GameRules(std::string filename);
    size_t getWidthField() { return _widthField; };
    size_t getHeightField() { return _heightField; };
    size_t getNumberOfShips() { return _numberOfShips; };
    size_t getTurnTime() { return _turnTime; };
    bool getFlagBonuses() { return _isActiveBonuses; };
    size_t getSquareOfShips() { return _squareOfShips; };
    Ship* getShip(size_t id);
};


#endif // WARSHIPS_GAMERULES_H
