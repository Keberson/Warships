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
    Ship(int id, std::string name, unsigned width, unsigned length, unsigned attackRadius) :
            _id(id), _name(name), _width(width), _length(length), _attackRadius(attackRadius) {};
    int getId() const { return _id; };
    std::string getName() { return _name; };
    unsigned getWidth() const { return _width; };
    unsigned getLength() const { return _length; };
    unsigned getAttackRadius() const { return _attackRadius; };
};

#endif // WARSHIPS_SHIP_H
