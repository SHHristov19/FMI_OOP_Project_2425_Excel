#pragma once
#include "Cell.h"
#include "IFormula.h"

class FormulaCell : public Cell 
{
private:
    IFormula* formula;

public:
    FormulaCell(IFormula* f) : formula(f) 
    {
        type = CellType::FORMULA;
    }

    FormulaCell(const FormulaCell& other) 
    {
        formula = other.formula->clone();
        type = CellType::FORMULA;
    }

    FormulaCell& operator=(const FormulaCell& other)
    {
        if (this != &other)
        {
            delete formula;
            formula = other.formula->clone();
            type = CellType::FORMULA;
        }
        return *this;
    }

    ~FormulaCell()
    {
        delete formula;
    }

    std::string evaluate() const override
    {
        return formula->evaluate(nullptr);
    }

    std::string toString() const override
    {
        return evaluate();
    }

    Cell* clone() const override
    {
        return new FormulaCell(*this);
    }

    void free() { delete formula; formula = nullptr; }

    void copyFrom(const Cell* other)
    {
        const FormulaCell* ptr = dynamic_cast<const FormulaCell*>(other);
        if (ptr)
        {
            formula = ptr->formula->clone();
        }
    }

    void moveFrom(Cell* other) {
        FormulaCell* ptr = dynamic_cast<FormulaCell*>(other);
        if (ptr) {
            formula = ptr->formula;
            ptr->formula = nullptr;
        }
    }
};
