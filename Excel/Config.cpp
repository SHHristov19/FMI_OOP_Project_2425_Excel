#include "Config.h"

bool Config::isDigitString(const std::string& str) const 
{
    for (char ch : str)
        if (!isdigit(ch)) return false;
    return !str.empty();
}

bool Config::isBool(const std::string& str) const 
{
    return str == "true" || str == "false";
}

bool Config::isAlignment(const std::string& str) const 
{
    return str == "left" || str == "center" || str == "right";
}

bool Config::loadFromFile(const std::string& filename) 
{
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    std::string line;
    while (std::getline(in, line)) 
    {
        if (line.empty()) continue;

        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) 
        {
            std::cerr << "ABORTING! " << line << " - Missing ':' delimiter!\n";
            return false;
        }

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);
        
        if (key == "initialTableRows") 
        {
            if (!isDigitString(value)) goto invalid;
            initialTableRows = value;
        }
        else if (key == "initialTableCols") 
        {
            if (!isDigitString(value)) goto invalid;
            initialTableCols = value;
        }
        else if (key == "maxTableRows") 
        {
            if (!isDigitString(value)) goto invalid;
            maxTableRows = value;
        }
        else if (key == "maxTableCols") 
        {
            if (!isDigitString(value)) goto invalid;
            maxTableCols = value;
        }
        else if (key == "autoFit") 
        {
            if (!isBool(value)) goto invalid;
            autoFit = value;
        }
        else if (key == "visibleCellSymbols") 
        {
            if (!isDigitString(value)) goto invalid;
            visibleCellSymbols = value;
        }
        else if (key == "initialAlignment") 
        {
            if (!isAlignment(value)) goto invalid;
            initialAlignment = value;
        }
        else if (key == "clearConsoleAfterCommand") 
        {
            if (!isBool(value)) goto invalid;
            clearConsoleAfterCommand = value;
        }
        else 
        {
            std::cerr << "ABORTING! " << key << ":" << value << " - Missing property!\n";
            return false;
        }

        continue;
    invalid:
        std::cerr << "ABORTING! " << key << ":" << value << " - Invalid value!\n";
        return false;
    }

    return true;
}

bool Config::saveToFile(const std::string& filename) const 
{
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    out << "initialTableRows:" << initialTableRows << "\n";
    out << "initialTableCols:" << initialTableCols << "\n";
    out << "maxTableRows:" << maxTableRows << "\n";
    out << "maxTableCols:" << maxTableCols << "\n";
    out << "autoFit:" << autoFit << "\n";
    out << "visibleCellSymbols:" << visibleCellSymbols << "\n";
    out << "initialAlignment:" << initialAlignment << "\n";
    out << "clearConsoleAfterCommand:" << clearConsoleAfterCommand << "\n";

    return true;
}

int Config::getInt(const std::string& key) const 
{
    if (key == "initialTableRows") return std::stoi(initialTableRows);
    if (key == "initialTableCols") return std::stoi(initialTableCols);
    if (key == "maxTableRows") return std::stoi(maxTableRows);
    if (key == "maxTableCols") return std::stoi(maxTableCols);
    if (key == "visibleCellSymbols") return std::stoi(visibleCellSymbols);
    return 0;
}

bool Config::getBool(const std::string& key) const 
{
    if (key == "autoFit") return autoFit == "true";
    if (key == "clearConsoleAfterCommand") return clearConsoleAfterCommand == "true";
    return false;
}

std::string Config::getString(const std::string& key) const 
{
    if (key == "initialAlignment") return initialAlignment;
    return "";
}

void Config::print() const 
{
    std::cout << "initialTableRows: " << initialTableRows << "\n";
    std::cout << "initialTableCols: " << initialTableCols << "\n";
    std::cout << "maxTableRows: " << maxTableRows << "\n";
    std::cout << "maxTableCols: " << maxTableCols << "\n";
    std::cout << "autoFit: " << autoFit << "\n";
    std::cout << "visibleCellSymbols: " << visibleCellSymbols << "\n";
    std::cout << "initialAlignment: " << initialAlignment << "\n";
    std::cout << "clearConsoleAfterCommand: " << clearConsoleAfterCommand << "\n";
}
