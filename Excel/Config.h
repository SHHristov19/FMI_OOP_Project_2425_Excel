#pragma once
#include <string>
#include <fstream>

class Config {
private:
    std::string initialTableRows;
    std::string initialTableCols;
    std::string maxTableRows;
    std::string maxTableCols;
    std::string autoFit;
    std::string visibleCellSymbols;
    std::string initialAlignment;
    std::string clearConsoleAfterCommand;

    bool isDigitString(const std::string& str) const;
    bool isBool(const std::string& str) const;
    bool isAlignment(const std::string& str) const;

public:
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;

    int getInt(const std::string& key) const;
    bool getBool(const std::string& key) const;
    std::string getString(const std::string& key) const;

    void print() const;
};
