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
    bool getFlagBonuses() { return _isActiveBonuses; };
    unsigned getSquareOfShips() { return _squareOfShips; };
    Ship* getShip(unsigned id);
    std::vector<Ship> getShips() { return _ships; };
    void setWidthField(unsigned width) { _widthField = width; };
    void setHeightField(unsigned height) { _heightField = height; };
};


#endif // WARSHIPS_GAMERULES_H
