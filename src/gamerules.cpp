#include <iostream>
#include <fstream>

#include "gamerules.h"

// Parser
std::string deleteSymbols(std::string str, std::string symbols) {
    for (auto symbol: symbols) {
        auto cPos = str.find(symbol);
        while (cPos != std::string::npos) {
            str = str.erase(cPos, 1);
            cPos = str.find(symbol);
        }
    }

    return str;
}

std::vector<std::pair<std::string, std::string>>* Parser::getParserPointer(std::string str) {
    if (str == "rules") {
        return &_rules;
    } else if (str == "ships") {
        return &_ships;
    } else if (str == "bonuses") {
        return &_bonuses;
    }

    return nullptr;
}


void Parser::parse() {
    std::fstream cgfile(_filename);
    std::string line;
    std::vector<std::pair<std::string, std::string>>* addressVariable = nullptr;

    if (cgfile.is_open()) {
        while (getline(cgfile, line)) {
            auto commentPos = line.find("//");
            if (commentPos != std::string::npos) {
                line = line.substr(0, commentPos);
            }

            auto symbolPos = line.find(':');
            if (symbolPos != std::string::npos) {
                auto beforeSymbol = line.substr(0, symbolPos);
                auto afterSymbol = line.substr(symbolPos+1);
                beforeSymbol = deleteSymbols(beforeSymbol, "\t ");
                afterSymbol = deleteSymbols(afterSymbol, "\t ");
                if (afterSymbol.empty()) {
                    addressVariable = getParserPointer(beforeSymbol);
                } else {
                    if (addressVariable != nullptr) {
                        addressVariable->push_back(std::make_pair(beforeSymbol, afterSymbol));
                    }
                }
            }
        }
    } else {
        cgfile.close();
        throw "Parser: can't open file";
    }

    cgfile.close();
}

std::vector<std::string> Parser::getParserItem(std::string chapter, std::string key) {
    auto address = getParserPointer(chapter);
    std::vector<std::string> tempVector;
    if (address == nullptr) {
        return {};
    }

    for (auto item: *address) {
        if (item.first == key) {
            tempVector.push_back(item.second);
        }
    }

    return tempVector;
}

std::ostream &operator<<(std::ostream &out, Parser &parser) {
    out << "--- chapter - rules ---" << std::endl;
    for (auto item: parser._rules) {
        out << item.first << ": " << item.second << std::endl;
    }

    out << std::endl << "--- chapter - ships ---" << std::endl;
    for (auto item: parser._ships) {
        out << item.first << ": " << item.second << std::endl;
    }

    out << std::endl << "--- chapter - bonuses ---" << std::endl;
    for (auto item: parser._bonuses) {
        out << item.first << ": " << item.second << std::endl;
    }

    return out;
}

// /Parser

// GameRules

GameRules::GameRules(std::string filename) {
    parser = Parser(filename);
    parser.parse();
    std::vector<std::string> tempVector;
    tempVector = parser.getParserItem("rules", "width");
    _widthField = (tempVector.empty() || std::stoi(tempVector[0]) <= 0) ? STANDARD_FIELD_WIDTH : std::stoi(tempVector[0]);
    tempVector = parser.getParserItem("rules", "height");
    _heightField = (tempVector.empty() || std::stoi(tempVector[0]) <= 0) ? STANDARD_FIELD_HEIGHT : std::stoi(tempVector[0]);
    tempVector = parser.getParserItem("rules", "numberOfShip");                                                                     //TODO(keberson): если один из параметров настройки кораблей неверный, то и все параметры неверны, поэтому включаем режим Стандарт
    _numberOfShips = (tempVector.empty() || std::stoi(tempVector[0]) <= 0) ? STANDARD_NUMBER_OF_SHIPS : std::stoi(tempVector[0]);
    tempVector = parser.getParserItem("rules", "typeOfShips");
    _isCustomShips = (tempVector.empty() || tempVector[0] != "inactive" || tempVector[0] != "active") ?
                        STANDARD_CUSTOM_SHIPS : (tempVector[0] == "custom");
    int idCounter = STANDARD_ID_START;
    unsigned j = 0;
    unsigned shipSwitch = 0;
    for (unsigned i = 0; i < _numberOfShips; ++i, ++j) {
        if (_isCustomShips) {
            // TODO(keberson): заполнение кастомными кораблями
        } else {
            if (j >= STANDARD_SHIPS_NUMBER[shipSwitch]) {
                j = 0;
                shipSwitch++;
            }

            // TODO(keberson): сортировка по width и length, чтобы с наибольшой площадью были в конце, а с наименьшим в начале
            _ships.push_back(Ship(idCounter++, STANDARD_SHIPS_NAME[shipSwitch], STANDARD_SHIPS_WIDTH[shipSwitch],
                                  STANDARD_SHIPS_HEIGHT[shipSwitch], STANDARD_SHIPS_ATTACK_RADIUS[shipSwitch]));
        }

        // TODO(keberson): переделать вычисление площади при использовании кастомных кораблей
        _squareOfShips += STANDARD_SHIPS_HEIGHT[shipSwitch]*STANDARD_SHIPS_WIDTH[shipSwitch];
    }

    tempVector = parser.getParserItem("rules", "turnTime");
    _turnTime = (tempVector.empty() || std::stoi(tempVector[0]) <= 0) ? STANDARD_TURN_TIME : std::stoi(tempVector[0]);
    tempVector = parser.getParserItem("rules", "bonuses");
    _isActiveBonuses = (tempVector.empty() || tempVector[0] != "inactive" || tempVector[0] != "active") ?
                        STANDARD_BONUSES : (tempVector[0] == "active");
    /*if (_isActiveBonuses) {
         tempVector = parser.getParserItem("rules", "isActiveBonuses");
        TODO(keberson): сделать заполнение вектора _bonuses бонусами
    }*/
}

Ship* GameRules::getShip(unsigned id) {
    for (int i = 0; i < _numberOfShips; ++i) {
        if (_ships[i].getId() == id) {
            return &_ships[i];
        }
    }

    return nullptr;
}
