#include "field.h"
#include "standards.h"

Field::Field(unsigned width, unsigned height) {
    _numberOfHits = 0;
    _width = width;
    _height = height;
    for (unsigned i = 0; i < width; ++i) {
        for (unsigned j = 0; j < height; ++j) {
            _cells.emplace_back(i, j, 0);
        }
    }
}

void Field::setID(unsigned x, unsigned y, unsigned id) {
    _cells[x * _width + y].setID(id);
    if (id >= STANDARD_ID_START + ID_SHIPS_OFFSET) {
        _numberOfHits++;
    }
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