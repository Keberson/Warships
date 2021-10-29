#ifndef WARSHIPS_UI_H
#define WARSHIPS_UI_H

#include <string>
#include <sstream>

#include "field.h"

class IUI {
public:
    virtual void displayMenu() = 0;
    virtual void displayFields(Field& leftField, Field& rightField) = 0;
    virtual void displayTheField(Field& field, unsigned offset) = 0;
    virtual void clearScreen() = 0;
    virtual ~IUI() {};
};

class ConsoleUI: public IUI {
public:
    ConsoleUI() {};
    void displayMenu() {};
    void displayFields(Field& leftField, Field& rightField);
    void displayTheField(Field& field, unsigned offset);
    void clearScreen();
    void setBackground();
    void setSeaCell();
    void setShippCell();
    void setDestroyedShip();
    void setMissCell();
    void setCursor(unsigned x, unsigned y);
    void turnOffCursor();
    unsigned comax();
    unsigned romax();
};


#endif // WARSHIPS_UI_H
