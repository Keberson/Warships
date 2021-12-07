#ifndef WARSHIPS_SHIP_H
#define WARSHIPS_SHIP_H

#include <string>

class Ship {
private:
    unsigned _id;
    std::string _name;
    unsigned _width;
    unsigned _length;
    unsigned _attackRadius;
    int _hp;
public:
    Ship() {};
    Ship(unsigned id, std::string name, unsigned width, unsigned length) :
            _id(id), _name(name), _width(width), _length(length) { _hp = width * length; };
    unsigned getId() { return _id; };
    std::string getName() { return _name; };
    unsigned getWidth() { return _width; };
    unsigned getLength() { return _length; };
    int getHP() { return _hp; };
    void attackShip(unsigned damage);
};

#endif // WARSHIPS_SHIP_H
