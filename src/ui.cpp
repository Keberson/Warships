#include <iostream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "ui.h"

struct termios saved_attributes;

void reset_input_mode (void) {
    tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void ConsoleUI::displayFields(Field& leftField, Field& rightField) {
    unsigned offset = 40;
    //unsigned y = leftField.getHeight()*2 + 2;
    displayTheField(leftField, 1, false);
    displayTheField(rightField, offset, true);
    setCursor(25, 1);
    std::cout << "\033[J";
    setCursor(24, 1);
}

void ConsoleUI::displayTheField(Field& field, unsigned offset, bool isHide) {
    unsigned space = 5;
    unsigned widthField = 31;         // romax() / 2 - space;    widthCell*10 + 11*1
    unsigned widthCell = 2;           // (widthField - (width + 1)) / numb;
    unsigned heightCell = 1;          // widthCell / 2;
    unsigned counterY = 0;
    unsigned rowCounter = 0;
    unsigned columnCounter = 0;

    bool isBlackRow = false;

    setCursor(1, offset);
    std::cout << "   a  b  c  d  e  f  g  h  i  j";
    while (rowCounter != (field.getHeight() + 1)) {
        setCursor(counterY + 2, offset);
        if (counterY % (heightCell + 1) == 0) {
            isBlackRow = true;
            rowCounter++;
        } else {
            isBlackRow = false;
            std::cout.width(3);
            std::cout << std::left << rowCounter;
        }

        columnCounter = 0;
        for (unsigned j = 1; j < widthField; j++) {
            if (isBlackRow) {
                setBackground();
            } else {
                if (j % (widthCell + 1) == 0) {
                    setBackground();
                    columnCounter++;
                } else if (field.getCell(rowCounter - 1, columnCounter).getID() == 0) {
                    setSeaCell();
                } else if (field.getCell(rowCounter - 1, columnCounter).getID() / 10 == 1) {
                    if (isHide) {
                        setSeaCell();
                    } else {
                        setShippCell();
                    }
                } else if (field.getCell(rowCounter - 1, columnCounter).getID() / 10 == 2) {
                    setDestroyedShip();
                } else if (field.getCell(rowCounter - 1, columnCounter).getID() % 10 == 2) {
                    setActiveCell();
                } else {
                    setMissCell();
                }
            }

        }

        setBackground();
        counterY++;
    }

    setBackground();
    std::cout << std::endl;
}

void ConsoleUI::clearScreen() {
    std::cout << "\033[2J";
}


void ConsoleUI::setInputMode() {
    struct termios tattr;
    tcgetattr (STDIN_FILENO, &saved_attributes);
    atexit(reset_input_mode);
    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON|ECHO);
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);

    turnOffCursor();
}

void ConsoleUI::setBackground() {
    std::cout << "\033[40m ";
}

void ConsoleUI::setSeaCell() {
    std::cout << "\033[44m ";
}

void ConsoleUI::setShippCell() {
    std::cout << "\033[42m ";
}

void ConsoleUI::setDestroyedShip() {
    std::cout << "\033[41m ";
}

void ConsoleUI::setMissCell() {
    std::cout << "\033[43m ";
}

void ConsoleUI::setActiveCell() {
    std::cout << "\033[47m ";
}

void ConsoleUI::setCursor(unsigned x, unsigned y) {
    std::cout << "\033[" << x << ";" << y << "H";
}

void ConsoleUI::turnOffCursor() {
    std::cout << "\033[?25l";
}

unsigned ConsoleUI::comax() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return(w.ws_col);
}

unsigned ConsoleUI::romax() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return(w.ws_row);
}
