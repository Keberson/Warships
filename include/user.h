#ifndef WARSHIPS_USER_H
#define WARSHIPS_USER_H

#include <string>
#include <vector>

#include "field.h"
#include "ui.h"

#define SIGNAL_TURN_MISS 0
#define SIGNAL_TURN_HIT 1
#define SIGNAL_GAME_EXIT 2

#define ERROR_OPEN 0
#define SUCCESS 1

class User {
protected:
    std::string _name;
    Field _field;

    std::vector<Cell> getCells() { return _field.getAllCells(); };
public:
    User() {};
    User(std::string name, unsigned widthField, unsigned heightField, std::vector<Ship> ships);
    void randomPlaceShip(unsigned id);
    void randomPlaceIsland(unsigned number);
    bool attackEnemy(unsigned x, unsigned y, Field& enemyField);
    Field& getField() { return _field; };
    virtual short turn(Field& enemyField, ConsoleUI& ui) = 0;
    short saveSession(std::string name, Field& enemyField);
    short loadSession(std::string name, Field& enemyField);
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
