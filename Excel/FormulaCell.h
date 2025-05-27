#pragma once
#include "Cell.h"
#include <string>
#include "FormulaType.h"

template<typename T>
class FormulaCell : public Cell
{
protected:
    T value;

public:
    FormulaCell(const T& val, FormulaType t) : value(val)
    {
        type = CellType::FORMULA;


    }

    virtual std::string convertToString() const
    {
        return "[unsupported]";
    }

    Cell* clone() const override
    {
        return new FormulaCell<T>(*this);
    }

    void free() {}

    void copyFrom(const Cell* other)
    {
        const FormulaCell<T>* ptr = dynamic_cast<const FormulaCell<T>*>(other);
        if (ptr) {
            value = ptr->value;
            type = ptr->type;
        }
    }

    void moveFrom(Cell* other)
    {
        FormulaCell<T>* ptr = dynamic_cast<FormulaCell<T>*>(other);
        if (ptr) {
            value = std::move(ptr->value);
            type = ptr->type;
        }
    }
};

// Specializations for different types

template<>
inline std::string FormulaCell<int>::convertToString() const
{
    return std::to_string(value);
}

template<>
inline std::string FormulaCell<double>::convertToString() const
{
    std::string str = std::to_string(value);
    str.erase(str.find_last_not_of('0') + 1);
    if (!str.empty() && str.back() == '.') str.pop_back();
    return str;
}

template<>
inline std::string FormulaCell<bool>::convertToString() const
{
    return value ? "TRUE" : "FALSE";
}

template<>
inline std::string FormulaCell<std::string>::convertToString() const
{
    return value;
}
