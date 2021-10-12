#include "field.h"
#include "standards.h"

Field::Field(size_t width, size_t height) {
    _numberOfHits = 0;
    _width = width;
    _height = height;
    for (size_t i = 0; i < width; ++i) {
        for (size_t j = 0; j < height; ++j) {
            _cells.emplace_back(i, j, 0);
        }
    }
}

void Field::setID(size_t x, size_t y, size_t id) {
    _cells[x*_width + y].setID(id);
    if (id >= STANDARD_ID_START + 10) {                     // TODO(keberson): изменить условие, должно быть не + 10
        _numberOfHits++;
    }
}

std::ostream& operator << (std::ostream& out, Field& field) {
    out.width(3);
    out << ' ';
    for (size_t i = 0; i < field.getWidth(); ++i) {                // TODO(keberson): предусмотреть случай, когда ширина больше, чем 26
        out.width(3);
        out << (char)('a' + i);
    }

    size_t counter = 0;
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