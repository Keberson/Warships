#include <iostream>
#include <sys/ioctl.h>
#include <string>
#include <sstream>
#include <termios.h>
#include <unistd.h>

#include "ui.h"

#define MENU_TEXT_COLOR 0
#define MENU_BACKGROUND_COLOR 6
#define MENU_ACTIVE_TEXT 7

struct termios savedAttributes;

unsigned MENU_INTERACTIVE_STRING_START = 3;

std::vector<std::string> MENU_RAW = {"", "Menu", "", "Start game", "Options", "Titles", "Exit", ""};

std::string FIRST_STRING_START_SYMBOL = "╔";
std::string FIRST_STRING_END_SYMBOL = "╗";
std::string LAST_STRING_START_SYMBOL = "╚";
std::string LAST_STRING_END_SYMBOL = "╝";
std::string INACTIVE_SYMBOL = "║";
std::string ACTIVE_SYMBOL_START = "╠";
std::string ACTIVE_SYMBOL_END = "╣";

class MenuString {          // TODO(keberson): подумать над строкой Action - действие при той или иной строке
private:
    std::string _text;
    std::string _finished;
    short _color;
    short _background;
    bool _isActive;
    unsigned _length;
public:
    MenuString() {};
    MenuString(std::string text, short color, short background, unsigned length) :
            _text(text), _color(color), _background(background), _length(length), _isActive(false) {};
    void setText(std::string text) { _text = text; };
    void setColor(short color) { _color = color; };
    void setBackground(short background) { _background = background; };
    void setLength(short length) { _length = length; };
    void buildString(std::string startSymbol = INACTIVE_SYMBOL, std::string endSymbol = INACTIVE_SYMBOL);
    void setActive();
    void setInactive();
    short getLength() { return _length; };
    bool getActiveFlag() { return _isActive; };
    friend std::ostream& operator << (std::ostream& out, MenuString& str);
};

void MenuString::buildString(std::string startSymbol, std::string endSymbol) {
    short length = (_length - _text.length()) / 2 + (((_length - _text.length()) % 2 == 1) ? 1 : 0);
    _finished += startSymbol;
    if (_text.empty()) {
        _finished.insert(_finished.end(), _length, ' ');
    } else {
        _finished.insert(_finished.end(), (_length - _text.length()) / 2, ' ');
        _finished += _text;
        _finished.insert(_finished.end(), length, ' ');
    }

    _finished += endSymbol;
}

void MenuString::setActive() {
    _finished.erase(0, INACTIVE_SYMBOL.length());
    _finished.insert(0, ACTIVE_SYMBOL_START);
    _finished.erase(_finished.length() - INACTIVE_SYMBOL.length(), INACTIVE_SYMBOL.length());
    _finished.insert(_finished.length() , ACTIVE_SYMBOL_END);
    _isActive = true;
}

void MenuString::setInactive() {
    _finished.erase(0, ACTIVE_SYMBOL_START.length());
    _finished.insert(0, INACTIVE_SYMBOL);
    _finished.erase(_finished.length() - ACTIVE_SYMBOL_END.length(), ACTIVE_SYMBOL_END.length());
    _finished.insert(_finished.length() , INACTIVE_SYMBOL);
    _isActive = false;
}

std::ostream& operator << (std::ostream& out, MenuString& str) {
    std::string stream;
    stream += "\033[2;4";
    stream += str._background + '0';
    if (str._isActive) {
        stream += ";3";
        stream += MENU_ACTIVE_TEXT + '0';
    } else {
        stream += ";3";
        stream += str._color + '0';
    }
    stream += "m";

    out << stream.c_str() << str._finished << "\033[0;37;40m";
    return out;
}

std::vector<MenuString> MENU;

void resetInputMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &savedAttributes);
}

// ------------------------------------------------------------- Menu -------------------------------------------------

void ConsoleUI::buildMenu() {
    short max_length = 0;
    for (auto item: MENU_RAW) {
        if (item.length() > max_length) {
            max_length = item.length();
        }
    }

    max_length += 4;

    for (unsigned i = 0; i < MENU_RAW.size(); ++i) {
        MenuString tempClass(MENU_RAW[i], MENU_TEXT_COLOR, MENU_BACKGROUND_COLOR, max_length);
        if (i == 0) {
            tempClass.buildString(FIRST_STRING_START_SYMBOL, FIRST_STRING_END_SYMBOL);
        } else if (i == MENU_RAW.size() - 1) {
            tempClass.buildString(LAST_STRING_START_SYMBOL, LAST_STRING_END_SYMBOL);
        } else {
            tempClass.buildString();
        }

        MENU.push_back(tempClass);
    }
}

void ConsoleUI::displayMenu() {
    unsigned rowCounter = romax() / 2 - 4;
    unsigned colCounter = comax() / 2;

    for (auto item: MENU) {
        setCursor(++rowCounter, colCounter - (item.getLength() / 2));
        std::cout << item;
    }

    std::cout << "\033[37m";
};

unsigned ConsoleUI::getMenuSize() {
    return MENU.size();
}

void ConsoleUI::menuDoRowActive(unsigned row) {
    MENU[row].setActive();
}

void ConsoleUI::menuDoRowInactive(unsigned row) {
    MENU[row].setInactive();
}

unsigned ConsoleUI::getMenuStartIndex() {
    return MENU_INTERACTIVE_STRING_START;
}

void ConsoleUI::displayOptionsMenu() {

}

void ConsoleUI::displayTitlesMenu() {
    unsigned usec = 300000;
    unsigned rows = romax();
    unsigned col = comax() / 2;
    std::vector<std::string> titles = { "The idea was invented by Kozov A.V.",
                                        "The code was written by Kuzov M.Y. and Mandzhiev D.Kh.",
                                        "The logic of the game was written by Kuzov M.Y.",
                                        "The interface was written by Mandzhiev D.Kh." };
    std::vector<int> stringRow;
    for (auto item: titles) {
        stringRow.push_back(rows);
    }

    for (unsigned i = 0; i < titles.size(); ++i) {
        for (unsigned j = 0; j < i; ++j) {
            setCursor(stringRow[0] + 1 + j, col - (titles[j].length() / 2));
            std::cout << "\033[2K";
        }

        for (unsigned j = 0; j < i + 1; ++j) {
            setCursor(stringRow[j]--, col - (titles[j].length() / 2));
            std::cout << titles[j] << std::flush;
        }

        usleep(usec);
    }

    while (stringRow.back() >= 1) {
        int i;

        for (i = 0; i < titles.size(); ++i) {
            setCursor(stringRow[0] + 1 + i, col - (titles[i].length() / 2));
            std::cout << "\033[2K";
        }

        for (i = 0; i < titles.size(); ++i) {
            if (stringRow[i] != 0) {
                setCursor(stringRow[i]--, col - (titles[i].length() / 2));
                std::cout << titles[i] << std::flush;
            }
        }

        usleep(usec);
    }
}

// ------------------------------------------------------------- Game -------------------------------------------------

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
    tcgetattr (STDIN_FILENO, &savedAttributes);
    atexit(resetInputMode);
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