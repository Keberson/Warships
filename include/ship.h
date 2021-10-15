#ifndef WARSHIPS_SHIP_H
#define WARSHIPS_SHIP_H

class Ship {
private:
    unsigned _id;
    std::string _name;
    unsigned _width;
    unsigned _length;
    unsigned _attackRadius;
public:
    Ship() {};
    Ship(unsigned id, std::string name, unsigned width, unsigned length, unsigned attackRadius) :
            _id(id), _name(name), _width(width), _length(length), _attackRadius(attackRadius) {};
    unsigned getId() { return _id; };
    std::string getName() { return _name; };
    unsigned getWidth() { return _width; };
    unsigned getLength() { return _length; };
    unsigned getAttackRadius() { return _attackRadius; };
};

#endif // WARSHIPS_SHIP_H
