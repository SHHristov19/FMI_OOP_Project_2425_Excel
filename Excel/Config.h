#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Config 
{
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
	void createConfigFile(const std::string& filename);
    void print() const;

    void setInitialTableRow(const std::string& rows);
    void setInitialTableCol(const std::string& cols);
    void setMaxTableRow(const std::string& rows);
    void setMaxTableCol(const std::string& cols);
    void setAutoFit(const std::string& fit);
    void setVisibleCellSymbols(const std::string& symbols);
    void setInitialAlignment(const std::string& alignment);
    void setClearConsoleAfterCommand(const std::string& clear);

	~Config() = default;
};
