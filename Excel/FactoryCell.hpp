// C++ Excel/FactoryCell.hpp
#pragma once 
#include "Container.hpp"
#include <string>
#include "FormulaCell.h"
#include "ReferenceCell.hpp"
#include "ValueCell.hpp"

class FactoryCell : public Cell
{
private:
    CellType cellType;
    std::string rawInput;
    Table* table = nullptr;

public:
    FactoryCell(const std::string& input, Table* parent)
        : rawInput(input), table(parent), cellType(parseType(input)) {
    }

    // Evaluate method acts as a factory:
    // It creates an instance of the correct cell type and calls its evaluate method.
    std::string evaluate() const override
    {
        Cell* cell = clone();
        std::string result = (cell) ? cell->evaluate() : "";
		cell->setRowValue(rawInput);
        delete cell;
        return result;
    }

    std::string getRawInput() const
    {
        return rawInput;
    }

    std::string toString() const override
    {
        return rawInput;
    }

    CellType getType() const override
    {
        return cellType;
    }

    // Factory based clone returns an instance of the proper cell type
    Cell* clone() const override
    {
        if (cellType == CellType::REFERENCE)
        {
            return new ReferenceCell(rawInput, table);
        }
        else if (cellType == CellType::FORMULA)
        {
            return new FormulaCell(rawInput, table);
        }
        else if (cellType == CellType::NUMBER)
        {
            return new ValueCell<double>(std::stod(rawInput), cellType);
        }
        else if (cellType == CellType::BOOL)
        {
            return new ValueCell<bool>(rawInput == "TRUE", cellType);
        }
        else if (cellType == CellType::TEXT || cellType == CellType::EMPTY)
        {
            return new ValueCell<std::string>(rawInput, cellType);
        }
        return nullptr;
    }

private:
    bool isNumber(const std::string& s) const
    {
        if (s.empty()) return false;
        for (char c : s)
        {
            if (!isdigit(c) && c != '.' && c != '-')
                return false;
        }
        return true;
    }

    bool isBool(const std::string& s) const
    {
        return s == "TRUE" || s == "FALSE";
    }

    bool isCellRef(const std::string& s) const
    {
        return s.find_first_of("=") >= 2 && s.size() - s.find_last_of("=") >= 2 && std::count(s.begin(), s.end(), '=') == 1;
    }

    bool isRangeRef(const std::string& s) const
    {
        return s.find(':') != std::string::npos;
    }

    // Parses the data to determine the appropriate cell type.
    CellType parseType(const std::string& data) const
    {
        if (data.empty())
        {
            return CellType::EMPTY;
        }
        else if (isNumber(data))
        {
            return CellType::NUMBER;
        }
        else if (isBool(data))
        {
            return CellType::BOOL;
        }
        else if (isCellRef(data))
        {
            return CellType::REFERENCE;
        }
        else if (data[0] == '=')
        {
            return CellType::FORMULA;
        }
        else
        {
            return CellType::TEXT;
        }
    }
};