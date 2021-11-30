#ifndef WARSHIPS_PARSER_H
#define WARSHIPS_PARSER_H

#include <string>
#include <vector>

std::string deleteSymbols(std::string str, std::string symbols);

class Parser {
private:
    std::string _filename;
    std::vector<std::pair<std::string, std::string>> _rules;
    std::vector<std::pair<std::string, std::string>> _ships;
    std::vector<std::pair<std::string, std::string>> _bonuses;
    std::vector<std::pair<std::string, std::string>>* getParserPointer(std::string);
public:
    Parser() {};
    Parser(std::string filename) : _filename(filename) {};
    void parse();
    std::vector<std::string> getParserItem(std::string chapter, std::string key);
    friend std::ostream& operator << (std::ostream& out, Parser& parser);
};

#endif // WARSHIPS_PARSER_H