#ifndef WARSHIPS_FIELD_H
#define WARSHIPS_FIELD_H

#include <iostream>
#include <string>
#include <vector>

#include "ship.h"

class Cell {
private:
    size_t _x;
    size_t _y;
    size_t _id;
public:
    Cell() {};
    Cell(size_t x, size_t y): _x(x), _y(y) {};
    Cell(size_t x, size_t y, size_t id): _x(x), _y(y), _id(id) {};
    size_t getX() { return _x; };
    size_t getY() { return _y; };
    size_t getID() { return _id; };
    void setID(size_t id) { _id = id; };
};

class Field {
private:
    std::vector<Cell> _cells;
    size_t _width;
    size_t _height;
    size_t _numberOfHits;
public:
    Field() { _numberOfHits = 0; };
    Field(size_t, size_t);
    void setID(size_t x, size_t y, size_t id);
    Cell& getCell(size_t x, size_t y) { return _cells[x*_width + y]; };
    size_t getWidth() { return _width; };
    size_t getHeight() { return _height; };
    size_t getNumberOfHits() { return _numberOfHits; };
    friend std::ostream& operator << (std::ostream& out, Field& field);
};

#endif // WARSHIPS_FIELD_H
