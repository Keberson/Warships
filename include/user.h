#ifndef WARSHIPS_USER_H
#define WARSHIPS_USER_H

#include <string>
#include <vector>

#include "field.h"

class User {
private:
    std::string _name;
    std::vector<Field> _field;
public:
    User() {};
    User(std::string name, size_t widthField, size_t heightField);
    void placeShip(size_t id , GameRules& rules);
    bool attackEnemy(size_t x, size_t y, Field& enemyField, size_t offset);
    Field& getField(size_t number) { return _field[number]; };
    virtual bool turn(Field&, size_t) = 0;
    ~User() {};
};

class Computer;

class Player: public User {
public:
    Player() : User() {};
    Player(std::string name, size_t widthField, size_t heightField) : User(name, widthField, heightField) {};
    bool turn(Field& enemyField, size_t numberOfShips);

    friend class Computer;
};

class Computer: public User {
public:
    Computer() : User() {};
    Computer(std::string name, size_t widthField, size_t heightField) : User(name, widthField, heightField) {};
    bool turn(Field& enemyField, size_t numberOfShips);

    friend class Player;
};

#endif // WARSHIPS_USER_H
