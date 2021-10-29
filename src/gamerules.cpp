#include "gamerules.h"

bool square_comparator(const void* first, const void* second) {

}

GameRules::GameRules(std::string filename) {
    parser = Parser(filename);
    parser.parse();
    std::vector<std::string> tempVector;
    tempVector = parser.getParserItem("rules", "width");
    _widthField = (tempVector.empty() || std::stoi(tempVector[0]) <= 0) ? STANDARD_FIELD_WIDTH : std::stoi(tempVector[0]);
    tempVector = parser.getParserItem("rules", "height");
    _heightField = (tempVector.empty() || std::stoi(tempVector[0]) <= 0) ? STANDARD_FIELD_HEIGHT : std::stoi(tempVector[0]);
    tempVector = parser.getParserItem("rules", "numberOfShip");                                                                     // TODO(keberson): Задача для 2ой версии: если один из параметров настройки кораблей неверный, то и все параметры неверны, поэтому включаем режим Стандарт
    _numberOfShips = (tempVector.empty() || std::stoi(tempVector[0]) <= 0) ? STANDARD_NUMBER_OF_SHIPS : std::stoi(tempVector[0]);
    tempVector = parser.getParserItem("rules", "typeOfShips");
    _isCustomShips = (tempVector.empty() || tempVector[0] != "inactive" || tempVector[0] != "active") ?
                        STANDARD_CUSTOM_SHIPS : (tempVector[0] == "custom");
    unsigned idCounter = STANDARD_ID_START;
    unsigned j = 0;
    unsigned shipSwitch = 0;
    _squareOfShips = 0;
    for (unsigned i = 0; i < _numberOfShips; ++i, ++j) {
        if (_isCustomShips) {

            // TODO(keberson): Задача для 2ой версии: заполнение кастомными кораблями и подсчёт площади кораблей

        } else {
            if (j >= STANDARD_SHIPS_NUMBER[shipSwitch]) {
                j = 0;
                shipSwitch++;
            }

            Ship temp_ship(idCounter++, STANDARD_SHIPS_NAME[shipSwitch], STANDARD_SHIPS_WIDTH[shipSwitch],
                           STANDARD_SHIPS_HEIGHT[shipSwitch], STANDARD_SHIPS_ATTACK_RADIUS[shipSwitch]);
            bool isEnd = true;
            unsigned counter = 0;
            for (auto item: _ships) {
                if (item.getWidth() * item.getLength() < STANDARD_SHIPS_WIDTH[shipSwitch] * STANDARD_SHIPS_HEIGHT[shipSwitch]) {
                    _ships.insert(_ships.begin() + counter, temp_ship);
                    isEnd = false;
                    break;
                }

                counter++;
            }

            if (isEnd) {
                _ships.push_back(temp_ship);
            }

            _squareOfShips += STANDARD_SHIPS_HEIGHT[shipSwitch]*STANDARD_SHIPS_WIDTH[shipSwitch];
        }
    }

    tempVector = parser.getParserItem("rules", "turnTime");
    _turnTime = (tempVector.empty() || std::stoi(tempVector[0]) <= 0) ? STANDARD_TURN_TIME : std::stoi(tempVector[0]);
    tempVector = parser.getParserItem("rules", "bonuses");
    _isActiveBonuses = (tempVector.empty() || tempVector[0] != "inactive" || tempVector[0] != "active") ?
                        STANDARD_BONUSES : (tempVector[0] == "active");
    /*if (_isActiveBonuses) {
         tempVector = parser.getParserItem("rules", "isActiveBonuses");
        TODO(keberson): Задача для 2ой версии: сделать заполнение вектора _bonuses бонусами
    }*/
}

Ship* GameRules::getShip(unsigned id) {
    for (unsigned i = 0; i < _numberOfShips; ++i) {
        if (_ships[i].getId() == id) {
            return &_ships[i];
        }
    }

    return nullptr;
}
