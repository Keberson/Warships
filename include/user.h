// WARSHIPS_USER_H

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
    User(std::string, unsigned, unsigned);
    void changeCellState(unsigned, unsigned, unsigned);
    void placeShip(unsigned, GameRules&);
    virtual int turn() = 0;

    // тестовый метод
    void outputField() { std::cout << _field[0] << std::endl; };
};

class Player: virtual public User {
public:
    Player() : User() {};
    Player(std::string name, unsigned widthField, unsigned heightField) : User(name, widthField, heightField) {};
    int turn();
};

class Computer: virtual public User {
public:
    Computer() : User() {};
    Computer(std::string name, unsigned widthField, unsigned heightField) : User(name, widthField, heightField) {};
    int turn();
};

#endif //WARSHIPS_USER_H
