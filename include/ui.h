#ifndef WARSHIPS_UI_H
#define WARSHIPS_UI_H

#include <string>
#include <sstream>

#include "field.h"

class IUI {
public:
    virtual void displayFields(Field* leftField, Field* rightField) = 0;
    virtual void displayTheField(Field* field, unsigned offset) = 0;
    virtual void changeCellState(unsigned x, unsigned y, unsigned id) = 0;
    virtual void outputResult() = 0;
    virtual void clearScreen() = 0;
    virtual std::string getCell() = 0;
    virtual ~IUI() {};
};

class ConsoleUI: public IUI {
public:
    ConsoleUI() {};
    void displayFields(Field* leftField, Field* rightField);
    void displayTheField(Field* field, unsigned offset);
    void changeCellState(unsigned x, unsigned y, unsigned id);
    void outputResult();
    void clearScreen();
    void setBackground();
    void setSeaCell();
    void setShippCell();
    void setDestroyedShip();
    void setMissCell();
    void setCursor(unsigned x, unsigned y);
    std::string getCell();                  // TODO(danza_): привязать этот метод к ходу и дописать этот метод
    unsigned comax();
};


#endif // WARSHIPS_UI_H
