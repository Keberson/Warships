#ifndef WARSHIPS_BONUS_H
#define WARSHIPS_BONUS_H

#include <string>
#include <vector>

class Bonus {
private:
    std::string _name;
    std::string _type;
    int _action;
public:
    Bonus(std::string name = "", std::string type = "", int action = 0): _name(name), _type(type), _action(action) {};
    std::string getName() { return _name; };
    std::string getType() { return _type; };
    int getAction() { return _action; };
};

#endif // WARSHIPS_BONUS_H
