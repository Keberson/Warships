// WARSHIPS_GAMERULES_H

#ifndef WARSHIPS_GAMERULES_H
#define WARSHIPS_GAMERULES_H

#include <string>
#include <vector>

#include "field.h"
#include "standards.h"

class Bonus {
private:
    std::string _name;
    std::string _type;
    int _action;
public:
    Bonus(std::string name = "", std::string type = "", int action = 0): _name(name), _type(type), _action(action) {};
    std::string getName() { return _name; };
    std::string getType() { return _type; };
    int getAction() { return _action; };
};

class Parser {
private:
    std::string _filename;
    std::vector<std::pair<std::string, std::string>> _rules;
    std::vector<std::pair<std::string, std::string>> _ships;
    std::vector<std::pair<std::string, std::string>> _bonuses;
    std::vector<std::pair<std::string, std::string>>* getParserPointer(std::string);
public:
    Parser() {};
    Parser(std::string filename) : _filename(filename) {};
    void parse();
    std::vector<std::string> getParserItem(std::string, std::string);
    friend std::ostream& operator << (std::ostream& out, Parser& parser);
};

class GameRules {
private:
    unsigned _widthField;
    unsigned _heightField;
    unsigned _numberOfShips;
    bool _isCustomShips;
    unsigned _turnTime;
    bool _isActiveBonuses;
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
    Ship* getShip(unsigned);
};


#endif // WARSHIPS_GAMERULES_H


