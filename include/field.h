#ifndef WARSHIPS_FIELD_H
#define WARSHIPS_FIELD_H

#include <iostream>
#include <string>
#include <vector>

#include "ship.h"

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
