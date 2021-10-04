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
    void placeShip(unsigned, GameRules&);
    void attackEnemy(unsigned, unsigned, Field&, unsigned);
    Field& getField(unsigned number) { return _field[number]; };
    virtual void turn(Field&, unsigned) = 0;
};

class Computer;

class Player: public User {
public:
    Player() : User() {};
    Player(std::string name, unsigned widthField, unsigned heightField) : User(name, widthField, heightField) {};
    void turn(Field&, unsigned);

    friend class Computer;
};

class Computer: public User {
public:
    Computer() : User() {};
    Computer(std::string name, unsigned widthField, unsigned heightField) : User(name, widthField, heightField) {};
    void turn(Field&, unsigned);

    friend class Player;
};

#endif //WARSHIPS_USER_H
