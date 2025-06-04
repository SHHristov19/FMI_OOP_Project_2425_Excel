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
            if (!isDigitString(value) || value.empty()) goto invalid;
            initialTableRows = value;
        }
        else if (key == "initialTableCols") 
        {
            if (!isDigitString(value) || value.empty()) goto invalid;
            initialTableCols = value;
        }
        else if (key == "maxTableRows") 
        {
            if (!isDigitString(value) || value.empty()) goto invalid;
            maxTableRows = value;
        }
        else if (key == "maxTableCols") 
        {
            if (!isDigitString(value) || value.empty()) goto invalid;
            maxTableCols = value;
        }
        else if (key == "autoFit") 
        {
            if (!isBool(value) || value.empty()) goto invalid;
            autoFit = value;
        }
        else if (key == "visibleCellSymbols") 
        {
            if (!isDigitString(value) || value.empty()) goto invalid;
            visibleCellSymbols = value;
        }
        else if (key == "initialAlignment") 
        {
            if (!isAlignment(value) || value.empty()) goto invalid;
            initialAlignment = value;
        }
        else if (key == "clearConsoleAfterCommand") 
        {
            if (!isBool(value) || value.empty()) goto invalid;
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

void Config::setInitialTableRow(const std::string& rows)
{
    if (isDigitString(rows))
    {
		initialTableRows = rows;
	} 
    else
    {
		std::cerr << "Invalid value for initialTableRows: " << rows << "\n";
	}
}

void Config::setInitialTableCol(const std::string& cols)
{
	if (isDigitString(cols))
	{
		initialTableCols = cols;
	}
	else
	{
		std::cerr << "Invalid value for initialTableCols: " << cols << "\n";
	}
}

void Config::setMaxTableRow(const std::string& rows)
{
	if (isDigitString(rows))
	{
		maxTableRows = rows;
	}
	else
	{
		std::cerr << "Invalid value for maxTableRows: " << rows << "\n";
	}
}

void Config::setMaxTableCol(const std::string& cols)
{
	if (isDigitString(cols))
	{
		maxTableCols = cols;
	}
	else
	{
		std::cerr << "Invalid value for maxTableCols: " << cols << "\n";
	}
}

void Config::setAutoFit(const std::string& fit)
{
	if (isBool(fit))
	{
		autoFit = fit;
	}
	else
	{
		std::cerr << "Invalid value for autoFit: " << fit << "\n";
	}
}

void Config::setVisibleCellSymbols(const std::string& symbols)
{
	if (isDigitString(symbols))
	{
		visibleCellSymbols = symbols;
	}
	else
	{
		std::cerr << "Invalid value for visibleCellSymbols: " << symbols << "\n";
	}
}

void Config::setInitialAlignment(const std::string& alignment)
{
	if (isAlignment(alignment))
	{
		initialAlignment = alignment;
	}
	else
	{
		std::cerr << "Invalid value for initialAlignment: " << alignment << "\n";
	}
}

void Config::setClearConsoleAfterCommand(const std::string& clear)
{
	if (isBool(clear))
	{
		clearConsoleAfterCommand = clear;
	}
	else
	{
		std::cerr << "Invalid value for clearConsoleAfterCommand: " << clear << "\n";
	}
}

void Config::createConfigFile(const std::string& filename)
{
    std::cout << "Enter the configurations: \n";

	std::cout << "initialTableRows (default 5): ";
	std::string rows;
	std::getline(std::cin, rows);
	if (rows.empty()) rows = "5";
	setInitialTableRow(rows);

	std::cout << "initialTableCols (default 5): ";
	std::string cols;
	std::getline(std::cin, cols); 
	if (cols.empty()) cols = "5";
	setInitialTableCol(cols);

	std::cout << "maxTableRows (default 10): ";
	std::string maxRows;
	std::getline(std::cin, maxRows);
	if (maxRows.empty()) maxRows = "10";
	setMaxTableRow(maxRows);

	std::cout << "maxTableCols (default 10): ";
	std::string maxCols;
	std::getline(std::cin, maxCols);
	if (maxCols.empty()) maxCols = "10";
	setMaxTableCol(maxCols);

	std::cout << "autoFit (true/false, default true): ";
	std::string autoFitInput;
	std::getline(std::cin, autoFitInput);
	if (autoFitInput.empty()) autoFitInput = "true";
	setAutoFit(autoFitInput);

	std::cout << "visibleCellSymbols (default 5): ";
	std::string visibleSymbols;
	std::getline(std::cin, visibleSymbols);
	if (visibleSymbols.empty()) visibleSymbols = "5";
	setVisibleCellSymbols(visibleSymbols);

	std::cout << "initialAlignment (left/center/right, default center): ";
	std::string alignment;
	std::getline(std::cin, alignment);
	if (alignment.empty()) alignment = "center";
	setInitialAlignment(alignment);

	std::cout << "clearConsoleAfterCommand (true/false, default true): ";
	std::string clearConsole;
	std::getline(std::cin, clearConsole);
	if (clearConsole.empty()) clearConsole = "true";
	setClearConsoleAfterCommand(clearConsole);

	saveToFile(filename);
}