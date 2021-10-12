#include <iostream>

#include "ui.h"

// TODO(danza_): сделать консольный UI

size_t ConsoleUI::displayFields(size_t width, size_t height) {
    return 0;
}

size_t ConsoleUI::displayTheField(size_t width, size_t height) {
    /*
    unsigned thickness = 2;     //
    unsigned rowWidth = thickness * 10 + thickness * 12;
    unsigned colWidth = thickness * 10 + thickness * 12;
    unsigned offset = 5;

    unsigned i;
    unsigned j;
    for (i = 1; i < colWidth; i++) {
        for (j = 1; j < rowWidth; j++) {
            if (((i / thickness) % thickness == 0) && ((j / thickness) % thickness == 0) &&
                    ((i / thickness) * (j / thickness)) != 0) {
                std::ostringstream ss;
                ss << "\033[" << i << ';' << j << "H\033[40m ";
                std::cout << ss.str();
            } else {
                std::ostringstream ss;
                ss << "\033[" << i << ';' << j << "H\033[47m ";
                std::cout << ss.str();
            }
        }
    }

    std::ostringstream ss;
    ss << "\033[" << i << ";0H\033[40m ";
    std::cout << ss.str();*/
    return 1;
}

size_t ConsoleUI::changeCellState(size_t x, size_t y, size_t id) {
    return 0;
}

size_t ConsoleUI::outputResult() {
    return 0;
}

void ConsoleUI::clearScreen() {
    std::cout << "\033[2J";
}

std::string ConsoleUI::getCell() {
    std::string tempStr;
    std::cin >> tempStr;
    return tempStr;
}