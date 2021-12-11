#ifndef WARSHIPS_FIELD_H
#define WARSHIPS_FIELD_H

#include <iostream>
#include <map>
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
    std::vector<Ship> _ships;
    std::map<unsigned, std::vector<Cell*>> _shipsBorders;
    unsigned _width;
    unsigned _height;
    unsigned _numberOfHits;
public:
    Field() { _numberOfHits = 0; };
    Field(unsigned width, unsigned height, std::vector<Ship> ships);
    Cell& getCell(unsigned x, unsigned y) { return _cells.at(y * _width + x); };
    Ship* getShip(unsigned id);
    std::vector<Ship> getAllShips() { return _ships; };
    unsigned getWidth() { return _width; };
    unsigned getHeight() { return _height; };
    unsigned getNumberOfHits() { return _numberOfHits; };
    void setNumberOfHits(unsigned square) { _numberOfHits += square; };
    std::vector<Cell*> getBorderCells(unsigned id) { return _shipsBorders[id]; };
    void setID(unsigned x, unsigned y, unsigned id);
    void addShipBorder(unsigned id, std::vector<Cell*> cells);
    friend std::ostream& operator << (std::ostream& out, Field& field);
};

#endif // WARSHIPS_FIELD_H
