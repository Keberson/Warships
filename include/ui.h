#ifndef WARSHIPS_UI_H
#define WARSHIPS_UI_H

#include <string>

class IUI {
public:
    virtual size_t displayFields(size_t width, size_t height) = 0;
    virtual size_t displayTheField(size_t width, size_t height) = 0;
    virtual size_t changeCellState(size_t x, size_t y, size_t id) = 0;
    virtual size_t outputResult() = 0;
    virtual void clearScreen() = 0;
    virtual std::string getCell() = 0;
    virtual ~IUI() {};
};

class ConsoleUI: public IUI {
public:
    ConsoleUI() {};
    size_t displayFields(size_t width, size_t height);
    size_t displayTheField(size_t width, size_t height);
    size_t changeCellState(size_t x, size_t y, size_t id);
    size_t outputResult();
    void clearScreen();
    std::string getCell();
};


#endif // WARSHIPS_UI_H
