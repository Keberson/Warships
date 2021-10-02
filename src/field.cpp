#include "field.h"

// Ship

// /Ship

// Cell

// /Cell

// Field

Field::Field(unsigned width, unsigned height) {
    _width = width;
    _height = height;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            _cells.emplace_back(i, j, 0);
        }
    }
}

std::ostream& operator << (std::ostream& out, Field& field) {
    unsigned y = 0;
    for (auto item: field._cells) {
        if (item.getY() != y) {
            y = item.getY();
            out << std::endl;
        }

        out << item.getID() << ' ';
    }
    return out;
}

// /Field
