// WARSHIPS_FIELD_H

#ifndef WARSHIPS_FIELD_H
#define WARSHIPS_FIELD_H

#include <iostream>
#include <string>
#include <vector>

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

class Cell {
private:
    unsigned _x;
    unsigned _y;
    unsigned _id;
public:
    Cell() {};
    Cell(unsigned x, unsigned y): _x(x), _y(y) {};
    Cell(unsigned x, unsigned y, unsigned id): _x(x), _y(y), _id(id) {};
    unsigned getX() { return _x; };
    unsigned getY() { return _y; };
    unsigned getID() { return _id; };
    void setID(unsigned id) { _id = id; };
};

class Field {
private:
    std::vector<Cell> _cells;
    unsigned _width;
    unsigned _height;
    unsigned _numberOfHits;
public:
    Field() { _numberOfHits = 0; };
    Field(unsigned, unsigned);
    void setID(unsigned x, unsigned y, unsigned id);
    Cell& getCell(unsigned x, unsigned y) { return _cells[x*_width + y]; };
    unsigned getWidth() { return _width; };
    unsigned getHeight() { return _height; };
    unsigned getNumberOfHits() { return _numberOfHits; };
    friend std::ostream& operator << (std::ostream& out, Field& field);
};

#endif // WARSHIPS_FIELD_H
