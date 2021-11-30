#ifndef WARSHIPS_UI_H
#define WARSHIPS_UI_H

#include <string>
#include <sstream>

#include "field.h"

class IUI {
public:
    virtual void displayMenu() = 0;
    virtual void displayFields(Field& leftField, Field& rightField) = 0;
    virtual void displayTheField(Field& field, unsigned offset, bool isHide) = 0;
    virtual void clearScreen() = 0;
    virtual ~IUI() {};
};

class ConsoleUI: public IUI {
public:
    ConsoleUI() {};
    // Menu
    void buildMenu();
    void displayMenu();
    unsigned getMenuSize();
    unsigned getOptionsSize();
    void menuDoRowActive(std::string chapter, unsigned row);
    void menuDoRowInactive(std::string chapter, unsigned row);
    void menuDoValueActive(unsigned row);
    void menuDoValueInactive(unsigned row);
    unsigned getMenuStartIndex();
    unsigned getOptionsStartIndex();
    void displayOptions();
    void displayTitles();
    void synchronizeOptionsRatio(std::vector<std::string>& ratios);
    void setOptionsRatio(unsigned row, std::string str);
    // Game
    void displayFields(Field& leftField, Field& rightField);
    void displayTheField(Field& field, unsigned offset, bool isHide);
    void clearScreen();
    void setInputMode();
    void setBackground();
    void setSeaCell();
    void setShippCell();
    void setDestroyedShip();
    void setMissCell();
    void setActiveCell();
    void setCursor(unsigned x, unsigned y);
    void turnOffCursor();
    unsigned comax();
    unsigned romax();
};

#endif // WARSHIPS_UI_H
