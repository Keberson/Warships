#include "field.h"
#include "standards.h"

// Ship

// /Ship

// Cell

// /Cell

// Field

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
    for (auto item: field._cells) {
        printf("%3d", item.getID());
        if (item.getY() == (field.getWidth() - 1)) {
            printf("\n");
        }
    }

    return out;
}

void Field::setID(unsigned int x, unsigned int y, unsigned int id) {
    _cells[x*_width + y].setID(id);
    if (id >= STANDARD_ID_START + 10) {                     // TODO(keberson): изменить условие, должно быть не + 10
        _numberOfHits++;
    }
}

// /Field
