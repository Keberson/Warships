#include <iostream>
#include <sys/ioctl.h>
#include <string>
#include <sstream>
#include <termios.h>
#include <unistd.h>

#include "standards.h"
#include "ui.h"

#define TITLES_DELAY 300000

#define MENU_TEXT_COLOR 0
#define MENU_BACKGROUND_COLOR 6
#define MENU_ACTIVE_TEXT 7
#define OPTIONS_ACTIVE_BACKGROUND_COLOR 3

#define MENU_INDENT 3

#define FIELD_SPACE 1
#define FIELD_INDENT 5
#define FIELD_CELL_WIDTH 2
#define FIELD_CELL_HEIGHT 1

struct termios savedAttributes;

unsigned MENU_INTERACTIVE_STRING_START = 3;
unsigned OPTIONS_INTERACTIVE_STRING_START = 3;

std::vector<std::string> TITLES = { "The idea was invented by Kozov A.V.",
                                    "The code was written by Kuzov M.Y. and Mandzhiev D.Kh.",
                                    "The logic of the game was written by Kuzov M.Y.",
                                    "The interface was written by Mandzhiev D.Kh.",
                                    "Pavlov P.S., Sham D.A., Kolotilin V.S., Sidorv A.V. and Krylov. A.S. helped in testing."};
// Two last strings always are not interactive
std::vector<std::string> MENU_RAW = {"", "Menu", "", "Start game", "Options", "Titles", "Exit", ""};
std::vector<std::string> OPTIONS_RAW = {"", "Options", "", "Width: ", "Height: ", "Exit", ""};

std::vector<std::string> OPTIONS_RATIOS_UI;

std::string FIRST_STRING_START_SYMBOL = "╔";
std::string FIRST_STRING_END_SYMBOL = "╗";
std::string LAST_STRING_START_SYMBOL = "╚";
std::string LAST_STRING_END_SYMBOL = "╝";
std::string INACTIVE_SYMBOL = "║";
std::string ACTIVE_SYMBOL_START = "╠";
std::string ACTIVE_SYMBOL_END = "╣";

class MenuString {
private:
    std::string _text;
    std::string _value;
    std::string _finished;
    std::string _startInactive;
    std::string _endInactive;
    std::string _startActive;
    std::string _endActive;
    short _color;
    short _activeColor;
    short _background;
    short _backgroundValue;
    bool _isActiveRow;
    bool _isActiveValue;
    unsigned _length;
public:
    MenuString() {};
    MenuString(std::string text, std::string value, short color, short background, short activeColor, short backgroundValue, unsigned length,
               std::string startInactive, std::string endInactive, std::string startActive, std::string endActive) :
            _text(text), _value(value), _color(color), _background(background), _activeColor(activeColor), _backgroundValue(backgroundValue),
            _length(length), _isActiveRow(false), _isActiveValue(false), _startInactive(startInactive), _endInactive(endInactive),
            _startActive(startActive), _endActive(endActive) {};
    void setText(std::string text) { _text = text; };
    void setValue(std::string value) { _value = value; };
    void setColor(short color) { _color = color; };
    void setBackground(short background) { _background = background; };
    void setBackgroundValue(short background) { _backgroundValue = background; };
    void setLength(short length) { _length = length; };
    void setValueFlag(bool flag) { _isActiveValue = flag; };
    void buildString(std::string alignment = "center");
    void setActive();
    void setInactive();
    short getLength() { return _length; };
    std::string getText() { return _text; };
    std::string getValue() { return _value; };
    std::string getFinishedStr() { return _finished; };
    bool getActiveFlag() { return _isActiveRow; };
    bool getActiveValueFlag() { return _isActiveValue; };
    friend std::ostream& operator << (std::ostream& out, MenuString& str);
};

void MenuString::buildString(std::string alignment) {
    std::string startSymbol = (_isActiveRow) ? _startActive : _startInactive;
    std::string endSymbol = (_isActiveRow) ? _endActive : _endInactive;
    short leftLength = (_length - _text.length() - _value.length()) / 2;
    short rightLength = (_length - _text.length() - _value.length()) / 2 + (_length - _text.length() - _value.length()) % 2;
    if (alignment == "left") {
        leftLength = MENU_INDENT;
        rightLength = MENU_INDENT;
    }

    _finished.clear();
    _finished += startSymbol;
    _finished.insert(_finished.end(), leftLength, ' ');

    if (!_text.empty()) {
        _finished += _text;
        if (alignment == "left") {
            _finished.insert(_finished.end(), _length - _value.length() - _text.length() - leftLength - rightLength, ' ');
        }

        _finished += _value;
    } else {
        _finished.insert(_finished.end(), _length - leftLength - rightLength, ' ');
    }

    _finished.insert(_finished.end(), rightLength, ' ');
    _finished += endSymbol;
}

void MenuString::setActive() {
    _finished.erase(0, _startInactive.length());
    _finished.insert(0, _startActive);
    _finished.erase(_finished.length() - _endInactive.length(), _endInactive.length());
    _finished.insert(_finished.length() , _endActive);
    _isActiveRow = true;
}

void MenuString::setInactive() {
    _finished.erase(0, _startActive.length());
    _finished.insert(0, _startInactive);
    _finished.erase(_finished.length() - _endActive.length(), _endActive.length());
    _finished.insert(_finished.length() , _endInactive);
    _isActiveRow = false;
}

std::ostream& operator << (std::ostream& out, MenuString& str) {
    std::string row = (str._isActiveValue) ? str._finished.substr(0, str._finished.find(str._value)) : str._finished;
    std::string stream;
    stream += "\033[2;4";
    stream += str._background + '0';
    if (str._isActiveRow) {
        stream += ";3";
        stream += str._activeColor + '0';
    } else {
        stream += ";3";
        stream += str._color + '0';
    }
    stream += "m";

    out << stream.c_str() << row;
    if (str._isActiveValue) {
        std::string anyStream;
        anyStream += "\033[2;4";
        anyStream += str._backgroundValue + '0';
        anyStream += "m";
        out << anyStream.c_str() << str._value;
        out << stream.c_str() << str._finished.substr(str._finished.find(str._value) + str._value.length());
    }

    out << "\033[0;37;40m";
    return out;
}

std::vector<MenuString> MENU;
std::vector<MenuString> OPTIONS;
std::vector<MenuString> SHIPS_SELECT;

void resetInputMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &savedAttributes);
    std::cout << "\033[2J";
}

char ConsoleUI::getSymbol() {
    tcflush(STDIN_FILENO, TCIFLUSH);
    return getchar();
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
        std::string startInactive = INACTIVE_SYMBOL;
        std::string endInactive = INACTIVE_SYMBOL;
        if (i == 0) {
            startInactive = FIRST_STRING_START_SYMBOL;
            endInactive = FIRST_STRING_END_SYMBOL;
        } else if (i == MENU_RAW.size() - 1) {
            startInactive = LAST_STRING_START_SYMBOL;
            endInactive = LAST_STRING_END_SYMBOL;
        }

        MenuString tempClass(MENU_RAW[i], "", MENU_TEXT_COLOR, MENU_BACKGROUND_COLOR, MENU_ACTIVE_TEXT, 0, max_length,
                             startInactive, endInactive, ACTIVE_SYMBOL_START, ACTIVE_SYMBOL_END);
        if (i == 0) {
            tempClass.buildString("center");
        } else if (i == MENU_RAW.size() - 1) {
            tempClass.buildString("center");
        } else {
            tempClass.buildString("center");
        }

        MENU.push_back(tempClass);
    }

    max_length = 0;
    for (unsigned i = 0; i < OPTIONS_RAW.size(); ++i) {
        if (OPTIONS_RAW[i].length() + OPTIONS_RATIOS_UI[i].length() > max_length) {
            max_length = OPTIONS_RAW[i].length() + OPTIONS_RATIOS_UI[i].length();
        }
    }

    max_length += 6;

    for (unsigned i = 0; i < OPTIONS_RAW.size(); ++i) {
        std::string startInactive = INACTIVE_SYMBOL;
        std::string endInactive = INACTIVE_SYMBOL;
        if (i == 0) {
            startInactive = FIRST_STRING_START_SYMBOL;
            endInactive = FIRST_STRING_END_SYMBOL;
        } else if (i == OPTIONS_RAW.size() - 1) {
            startInactive = LAST_STRING_START_SYMBOL;
            endInactive = LAST_STRING_END_SYMBOL;
        }

        MenuString tempClass(OPTIONS_RAW[i], OPTIONS_RATIOS_UI[i], MENU_TEXT_COLOR, MENU_BACKGROUND_COLOR,
                             MENU_ACTIVE_TEXT, OPTIONS_ACTIVE_BACKGROUND_COLOR, max_length, startInactive,
                             endInactive,ACTIVE_SYMBOL_START, ACTIVE_SYMBOL_END);
        if (i == 0) {
            tempClass.buildString();
        } else if (i == OPTIONS_RAW.size() - 2) {
            tempClass.buildString("center");
        } else if (i == OPTIONS_RAW.size() - 1) {
            tempClass.buildString();
        } else if (i == 1) {
            tempClass.buildString("center");
        } else {
            tempClass.buildString("center");
        }

        OPTIONS.push_back(tempClass);
    }
}

void ConsoleUI::displayMenu() {
    unsigned rowCounter = romax() / 2 - 4;
    unsigned colCounter = comax() / 2;

    for (auto item: MENU) {
        setCursor(++rowCounter, colCounter - (item.getLength() / 2));
        std::cout << item << std::flush;
    }

    std::cout << "\033[37m";
};

unsigned ConsoleUI::getMenuSize() {
    return MENU.size();
}

unsigned ConsoleUI::getOptionsSize() {
    return OPTIONS.size();
}


void ConsoleUI::menuDoRowActive(std::string chapter, unsigned row) {
    if (chapter == "menu") {
        MENU[row].setActive();
    } else if (chapter == "options") {
        OPTIONS[row].setActive();
    }
}

void ConsoleUI::menuDoRowInactive(std::string chapter, unsigned row) {
    if (chapter == "menu") {
        MENU[row].setInactive();
    } else if (chapter == "options") {
        OPTIONS[row].setInactive();
    }
}

void ConsoleUI::menuDoValueActive(unsigned row) {
    OPTIONS[row].setValueFlag(true);
}

void ConsoleUI::menuDoValueInactive(unsigned row) {
    OPTIONS[row].setValueFlag(false);
}

unsigned ConsoleUI::getMenuStartIndex() {
    return MENU_INTERACTIVE_STRING_START;
}

unsigned ConsoleUI::getOptionsStartIndex() {
    return OPTIONS_INTERACTIVE_STRING_START;
}

void ConsoleUI::displayOptions() {
    unsigned rowCounter = romax() / 2 - 4;
    unsigned colCounter = comax() / 2;

    for (auto item: OPTIONS) {
        setCursor(++rowCounter, colCounter - (item.getLength() / 2));
        std::cout << item << std::flush;
    }

    std::cout << "\033[37m";
}

void ConsoleUI::displayTitles() {
    std::vector<int> stringRow;
    for (auto item: TITLES) {
        stringRow.push_back(romax());
    }

    for (unsigned i = 0; i < TITLES.size(); ++i) {
        for (unsigned j = 0; j < i; ++j) {
            setCursor(stringRow[0] + 1 + j, comax() / 2 - (TITLES[j].length() / 2));
            std::cout << "\033[2K";
        }

        for (unsigned j = 0; j < i + 1; ++j) {
            setCursor(stringRow[j]--, comax() / 2 - (TITLES[j].length() / 2));
            std::cout << TITLES[j] << std::flush;
        }

        usleep(TITLES_DELAY);
    }

    while (stringRow.back() >= 1) {
        int i;

        for (i = 0; i < TITLES.size(); ++i) {
            setCursor(stringRow[0] + 1 + i, comax() / 2 - (TITLES[i].length() / 2));
            std::cout << "\033[2K";
        }

        for (i = 0; i < TITLES.size(); ++i) {
            if (stringRow[i] != 0) {
                setCursor(stringRow[i]--, comax() / 2 - (TITLES[i].length() / 2));
                std::cout << TITLES[i] << std::flush;
            }
        }

        usleep(TITLES_DELAY);
    }


}

void ConsoleUI::setOptionsRatio(unsigned row, std::string ratio) {
    OPTIONS_RATIOS_UI[row] = ratio;
    OPTIONS[row].setValue(ratio);
    OPTIONS[row].buildString();
}

void ConsoleUI::synchronizeOptionsRatio(std::vector<std::string>& ratios) {
    OPTIONS_RATIOS_UI = ratios;
}

// ------------------------------------------------------------- Game -------------------------------------------------

void ConsoleUI::prepareShipSelect(Field& field) {
    SHIPS_SELECT.clear();
    std::vector<Ship> allShips = field.getAllShips();
    for (int i = 0; i < allShips.size(); ++i) {
        std::string tempStr;
        tempStr += allShips[i].getName();
        tempStr += " " + std::to_string(allShips[i].getLength()) + " x " + std::to_string(allShips[i].getWidth());
        SHIPS_SELECT.push_back(MenuString(tempStr, "", 7, 0, 2,
                                          0, tempStr.length(), "", "","",
                                          ""));
        SHIPS_SELECT[i].buildString();
    }
}

void ConsoleUI::displayShipsSelect(Field& field) {
    displayTheField(field, "left", false, "isPrepareState");
    unsigned rowCounter = 3;
    unsigned column = field.getWidth() * FIELD_CELL_WIDTH + (field.getWidth() + 1) * FIELD_SPACE + FIELD_INDENT;
    for (auto item: SHIPS_SELECT) {
        setCursor(rowCounter, column);
        std::cout << item << std::flush;
        rowCounter++;
    }
}

unsigned ConsoleUI::getShipSelectSize() {
    return SHIPS_SELECT.size();
}

void ConsoleUI::setShipDoRowActive(unsigned int row) {
    SHIPS_SELECT[row].setActive();
}

void ConsoleUI::setShipDoRowInactive(unsigned int row) {
    SHIPS_SELECT[row].setInactive();
}

void ConsoleUI::setShipDoRowFilled(unsigned row) {
    SHIPS_SELECT[row].setColor(1);
}

void ConsoleUI::displayFields(Field& leftField, Field& rightField, std::string options) {
    displayTheField(leftField, "left", false);
    displayTheField(rightField, "right", true, options);
}

void ConsoleUI::displayTheField(Field& field, std::string position, bool isHide, std::string options) {
    const unsigned width = field.getWidth();
    unsigned widthField = width * (FIELD_CELL_WIDTH + FIELD_SPACE);
    unsigned counterY = 0;
    unsigned rowCounter = 0;
    unsigned columnCounter = 0;
    unsigned offset = 0;
    bool isBlackRow = false;
    bool isPlayerTurn = false;
    bool isComputerTurn = false;
    bool isPrepareState = false;
    bool isFirstTurn = false;
    bool isMiss = false;
    bool isHit= false;
    bool isDestroyed = false;
    bool isInvalidCell = false;
    bool isSaveLoad = false;
    bool isOpened = false;
    std::string cell = "";

    if (options.find("isPrepareState") != std::string::npos) {
        isPrepareState = true;
    } else if (options.find("player") != std::string::npos) {
        isPlayerTurn = true;
    } else if (options.find("computer") != std::string::npos) {
        isComputerTurn = true;
    }

    if (options.find("isFirstTurn") != std::string::npos) {
        isFirstTurn = true;
    } else if (options.find("isMiss") != std::string::npos) {
        isMiss = true;
    } else if (options.find("isHit") != std::string::npos) {
        isHit = true;
    } else if (options.find("isDestroyed") != std::string::npos) {
        isDestroyed = true;
    } else if (options.find("isInvalidCell") != std::string::npos) {
        isInvalidCell = true;
    } else if (options.find("isSaveLoad") != std::string::npos) {
        isSaveLoad = true;
    } else if (options.find("isOpened") != std::string::npos) {
        isOpened = true;
    }

    auto name = options.find("cell:");
    if (name != std::string::npos) {
        cell = options.substr(name + 5);
        cell = cell.substr(0, options.find(';') - (name + 5));
    }

    if (position == "left") {
        offset = 1;
    } else if (position == "right") {
        offset = widthField + FIELD_INDENT;
    }

    setCursor(1, offset);
    std::cout << "   ";
    for (int i = 0; i < width; ++i) {
        std::cout << (char)('a' + i) << "  ";
    }

    while (rowCounter != (field.getHeight() + 1)) {
        setCursor(counterY + 2, offset);
        if (counterY % (FIELD_CELL_HEIGHT + 1) == 0) {
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
                if (j % (FIELD_CELL_WIDTH + 1) == 0) {
                    setBackground();
                    columnCounter++;
                } else if (field.getCell(columnCounter, rowCounter - 1).getID() == EMPTY_CELL_ID) {
                    setSeaCell();
                } else if (field.getCell(columnCounter, rowCounter - 1).getID() == ISLAND_ID_ATTACKED) {
                    setIslandCell();
                } else if (field.getCell(columnCounter, rowCounter - 1).getID() == ISLAND_ID) {
                    if (isHide) {
                        setSeaCell();
                    } else {
                        setIslandCell();
                    }
                } else if (field.getCell(columnCounter, rowCounter - 1).getID() / 10 == 1) {
                    if (isHide) {
                        setSeaCell();
                    } else {
                        setShipCell();
                    }
                } else if (field.getCell(columnCounter, rowCounter - 1).getID() / 10 == 2) {
                    setDestroyedShip();
                } else if (field.getCell(columnCounter, rowCounter - 1).getID() % 10 == 2) {
                    setActiveCell();
                } else if (field.getCell(columnCounter, rowCounter - 1).getID() % 10 == 5) {
                    setSelectShip();
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
    if (isFirstTurn) {
        std::cout << "\033[s" << "\033[J" << "\033[u";
    }

    if (isPrepareState) {
        std::cout << "\033[32m" << "Use the arrows (← ↑ ↓ →) to move selected ship around the playing field, press "
                  << "Enter when you select the cells, press R to rotate ship, press P to use random set" << "\033[37m";
    }

    if (isPlayerTurn) {
        if (isFirstTurn) {
            std::cout << "\033[2K" << "Now your turn..." << std::endl;
            std::cout << "Attack this cell: " << "\033[s" << " " << std::endl << std::endl;
            std::cout << "\033[32m" << "Use the arrows (← ↑ ↓ →) to move around the playing field, press "
                      << "Enter when you select the desired cell, press S to save the session and L to load the session" << "\033[37m";
        } else if (isMiss) {
            std::cout << "\033[2K" << "\033[33m" << "Miss..." << "\033[37m" << std::endl;
        } else if (isHit) {
            std::cout << "\033[2K" << "\033[31m" << "Hit!" << "\033[37m" << std::endl;
        } else if (isDestroyed) {
            std::cout << "\033[2K" << "\033[31m" << "Destroyed!" << "\033[37m" << std::endl;
        } else if (isInvalidCell) {
            std::cout << "\033[2K" << "\033[36m" << "Try again" << "\033[37m" << std::endl;;
        } else if (isSaveLoad) {
            std::cout << "\033[2K" << "\033[32m" << "Input filename: " << options.substr(options.find("isSaveLoad") + 11) << "\033[37m";
        } else if (isOpened) {
            std::cout << "\033[2K" << "\033[32m" << options.substr(options.find("isOpened") + 8) << "\033[37m";
        }

        std::cout << "\033[u" << "\033[K" << cell;
    }

    if (isComputerTurn) {
        if (isFirstTurn) {
            std::cout << "\033[2K" << "Computer is attacking" << std::endl;
        } else if (isMiss) {
            std::cout << "\033[2K" << "\033[33m" << "Miss..." << "\033[37m" << std::endl;
        } else if (isHit) {
            std::cout << "\033[2K" << "\033[31m" << "Hit!" << "\033[37m" << std::endl;
        } else if (isDestroyed) {
            std::cout << "\033[2K" << "\033[31m" << "Destroyed!" << "\033[37m" << std::endl;
        }
    }
}

void ConsoleUI::displayWinnerMessage(std::string winner) {
    std::string winStr;
    winStr += "Congratulations! " + winner + " is winner!";
    setCursor(romax() / 2, comax() / 2 - winStr.length() / 2);
    std::cout << winStr << std::endl;
}


void ConsoleUI::clearScreen() {
    setCursor(1,1);
    std::cout << "\033[J";
    setCursor(1,1);
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

void ConsoleUI::setShipCell() {
    std::cout << "\033[42m ";
}

void ConsoleUI::setDestroyedShip() {
    std::cout << "\033[101m ";
}

void ConsoleUI::setMissCell() {
    std::cout << "\033[104m ";      // \033[106m
}

void ConsoleUI::setActiveCell() {
    std::cout << "\033[47m ";
}

void ConsoleUI::setSelectShip() {
    std::cout << "\033[106m ";
}

void ConsoleUI::setIslandCell() {
    std::cout << "\033[43m ";
}

void ConsoleUI::setCursor(unsigned x, unsigned y) {
    std::cout << "\033[" << x << ";" << y << "H";
}

void ConsoleUI::turnOffCursor() {
    std::cout << "\033[?25l";
}

unsigned ConsoleUI::comax() {
    struct winsize ww;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ww);
    return ww.ws_col;
}

unsigned ConsoleUI::romax() {
    struct winsize ww;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ww);
    return ww.ws_row;
}