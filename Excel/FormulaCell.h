#pragma once 
#include <iostream>
#include "Table.h"
#include "FormulaType.h"

class FormulaCell : public Cell 
{
private:
    std::string rawFormula;
    Table* parentTable;

public:
    FormulaCell(const std::string& formulaText, Table* table)
        : rawFormula(formulaText), parentTable(table) 
    {
        type = CellType::FORMULA;
    };

    Cell* clone() const override;
    std::string evaluate() const override;
    std::string toString() const override;

private:
    std::string evalFormula(const std::string& formula) const;
    std::string evalFunction(const std::string& name, const container<std::string>& args) const;
    bool isCellRef(const std::string& s) const;
    bool isRange(const std::string& s) const;
    bool isNumber(const std::string& s) const;
    bool isBool(const std::string& s) const;
    std::string getCellValue(const std::string& ref) const;
    container<std::string> extractRange(const std::string& range) const;
};