#include <iostream>
#include <fstream>

#include "parser.h"

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
    std::ifstream cgfile(_filename.c_str());
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
                afterSymbol = deleteSymbols(afterSymbol, "\t \n\r");
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
        exit(-2);
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