#ifndef WARSHIPS_UI_H
#define WARSHIPS_UI_H

#include <string>

class IUI {
public:
    virtual int displayFields(unsigned, unsigned) = 0;
    virtual int displayTheField(unsigned, unsigned) = 0;
    virtual int changeCellState(unsigned, unsigned, int) = 0;
    virtual int outputResult() = 0;
    virtual void clearScreen() = 0;
    virtual std::string getCell() = 0;
    virtual ~IUI() {};
};

class ConsoleUI: public IUI {
public:
    ConsoleUI() {};
    int displayFields(unsigned, unsigned);
    int displayTheField(unsigned, unsigned);
    int changeCellState(unsigned, unsigned, int);
    int outputResult();
    void clearScreen();
    std::string getCell();
};


#endif // WARSHIPS_UI_H
