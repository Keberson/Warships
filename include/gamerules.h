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
    unsigned _widthField;
    unsigned _heightField;
    unsigned _numberOfShips;
    bool _isCustomShips;
    unsigned _turnTime;
    bool _isActiveBonuses;
    unsigned _squareOfShips;
    std::vector<Bonus> _bonuses;
    std::vector<Ship> _ships;
    Parser parser;
public:
    GameRules() {};
    GameRules(std::string filename);
    unsigned getWidthField() { return _widthField; };
    unsigned getHeightField() { return _heightField; };
    unsigned getNumberOfShips() { return _numberOfShips; };
    unsigned getTurnTime() { return _turnTime; };
    bool getFlagBonuses() { return _isActiveBonuses; };
    unsigned getSquareOfShips() { return _squareOfShips; };
    Ship* getShip(unsigned id);
};


#endif // WARSHIPS_GAMERULES_H
