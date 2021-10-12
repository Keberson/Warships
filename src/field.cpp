#include "field.h"
#include "ship.h"
#include "standards.h"

Field::Field(unsigned width, unsigned height) {
    _numberOfHits = 0;
    _width = width;
    _height = height;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            _cells.emplace_back(i, j, 0);
        }
    }
}

std::ostream& operator << (std::ostream& out, Field& field) {
    out.width(3);
    out << ' ';
    for (int i = 0; i < field.getWidth(); ++i) {                // TODO(keberson): предусмотреть случай, когда ширина больше, чем 26
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

void Field::setID(unsigned int x, unsigned int y, unsigned int id) {
    _cells[x*_width + y].setID(id);
    if (id >= STANDARD_ID_START + 10) {                     // TODO(keberson): изменить условие, должно быть не + 10
        _numberOfHits++;
    }
}

// /Field
