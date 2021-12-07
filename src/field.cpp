#include "field.h"
#include "standards.h"

Field::Field(unsigned width, unsigned height, std::vector<Ship> ships) {
    _numberOfHits = 0;
    _width = width;
    _height = height;
    _ships = ships;
    for (unsigned i = 0; i < width; ++i) {
        for (unsigned j = 0; j < height; ++j) {
            _cells.emplace_back(i, j, 0);
        }
    }
}

void Field::setID(unsigned x, unsigned y, unsigned id) {
    _cells[x * _width + y].setID(id);
}

Ship* Field::getShip(unsigned int id) {
    for (unsigned i = 0; i < _ships.size(); ++i) {
        if (_ships[i].getId() == id) {
            return &_ships[i];
        }
    }

    return nullptr;
}


std::ostream& operator << (std::ostream& out, Field& field) {       // Helper method
    out.width(3);
    out << ' ';
    for (unsigned i = 0; i < field.getWidth(); ++i) {
        out.width(3);
        out << (char)('a' + i);
    }

    unsigned counter = 0;
    for (auto item: field._cells) {
        if (item.getY() % field.getWidth() == 0) {
            out << std::endl;
            counter++;
            out.width(3);
            out << counter;
        }

        out.width(3);
        out << item.getID();
    }

    out << std::endl;
    return out;
}

void Field::addShipBorder(unsigned int id, std::vector<Cell*> cells) {
    _shipsBorders[id] = cells;
}
