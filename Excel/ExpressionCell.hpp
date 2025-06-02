#pragma once 
#include "Container.hpp"
#include <string>
#include "FormulaCell.h"
#include "ReferenceCell.hpp"
#include "ValueCell.hpp"

class ExpressionCell : public Cell 
{ 
private:
    CellType cellType;
    std::string rawInput;
    Table* table = nullptr;

public:
    ExpressionCell(const std::string& input, Table* parent)
        : rawInput(input), table(parent), cellType(parseType(input)) {};

    std::string evaluate() const override 
    {
        switch (cellType) 
        {
            case CellType::EMPTY: 
                return "";
            case CellType::TEXT:
            case CellType::NUMBER:
            case CellType::BOOL:
                return evalValue(cellType);
            case CellType::REFERENCE: 
                return evalReference();
            case CellType::FORMULA:
                return evalFormula();
		    case CellType::ERROR: 
                return "#VALUE!";
            default: 
                return "#VALUE!";
        }
    }

    std::string toString() const override 
    {
        return rawInput;
    }

    CellType getType() const override 
    {
        return cellType;
	}

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
        else
        {
            return new ExpressionCell(*this);
        }
    }

private:

    std::string evalValue(CellType type) const 
    {
        ValueCell<std::string> cell(rawInput, type);
		return cell.evaluate();
    }

    std::string evalReference() const 
    {
		ReferenceCell cell(rawInput, table);
        return cell.evaluate();
    }

    std::string evalRange() const 
    {
        return "#VALUE!";
    }

    std::string evalFormula() const 
    {
        FormulaCell cell(rawInput, table);
        return cell.evaluate();
    }

    bool isNumber(const std::string& s) const 
    {
        if (s.empty()) return false;
        for (char c : s) if (!isdigit(c) && c != '.' && c != '-') return false;
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
