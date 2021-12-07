#ifndef WARSHIPS_USER_H
#define WARSHIPS_USER_H

#include <string>
#include <vector>

#include "field.h"
#include "ui.h"

#define SIGNAL_TURN_MISS 0
#define SIGNAL_TURN_HIT 1
#define SIGNAL_GAME_EXIT 2

class User {
private:
    std::string _name;
    Field _field;
public:
    User() {};
    User(std::string name, unsigned widthField, unsigned heightField, std::vector<Ship> ships);
    void placeShip(unsigned id , GameRules& rules);
    bool attackEnemy(unsigned x, unsigned y, Field& enemyField);
    Field& getField() { return _field; };
    virtual short turn(Field& enemyField, ConsoleUI& ui) = 0;
    ~User() {};
};

class Computer;

class Player: public User {
public:
    Player() : User() {};
    Player(std::string name, unsigned widthField, unsigned heightField, std::vector<Ship> ships) : User(name, widthField, heightField, ships) {};
    short turn(Field& enemyField, ConsoleUI& ui);

    friend class Computer;
};

class Computer: public User {
public:
    Computer() : User() {};
    Computer(std::string name, unsigned widthField, unsigned heightField, std::vector<Ship> ships) : User(name, widthField, heightField, ships) {};
    short turn(Field& enemyField, ConsoleUI& ui);

    friend class Player;
};

#endif // WARSHIPS_USER_H
