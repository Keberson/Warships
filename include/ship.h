#ifndef WARSHIPS_SHIP_H
#define WARSHIPS_SHIP_H

class Ship {
private:
    size_t _id;
    std::string _name;
    size_t _width;
    size_t _length;
    size_t _attackRadius;
public:
    Ship() {};
    Ship(size_t id, std::string name, size_t width, size_t length, size_t attackRadius) :
            _id(id), _name(name), _width(width), _length(length), _attackRadius(attackRadius) {};
    size_t getId() { return _id; };
    std::string getName() { return _name; };
    size_t getWidth() { return _width; };
    size_t getLength() { return _length; };
    size_t getAttackRadius() { return _attackRadius; };
};

#endif // WARSHIPS_SHIP_H
