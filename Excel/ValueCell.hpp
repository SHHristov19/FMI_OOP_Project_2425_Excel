#pragma once
#include "Cell.h"
#include <string>

template<typename T>
class ValueCell : public Cell
{
protected:
    T value;

public:
    ValueCell(const T& val, CellType t) : value(val)
    {
        type = t;
    }
    
    std::string evaluate() const override
    {
        return convertToString();
    }

    std::string toString() const override
    {
        return evaluate();
    }

    virtual std::string convertToString() const
    {
        return "[unsupported]";
    }

    Cell* clone() const override
    {
        return new ValueCell<T>(*this);
    }

    void free() {}

    void copyFrom(const Cell* other)
    {
        const ValueCell<T>* ptr = dynamic_cast<const ValueCell<T>*>(other);
        if (ptr) {
            value = ptr->value;
            type = ptr->type;
        }
    }

    void moveFrom(Cell* other)
    {
        ValueCell<T>* ptr = dynamic_cast<ValueCell<T>*>(other);
        if (ptr) {
            value = std::move(ptr->value);
            type = ptr->type;
        }
    }
};

// Specializations for different types

template<>
inline std::string ValueCell<int>::convertToString() const
{
    return std::to_string(value);
}

template<>
inline std::string ValueCell<double>::convertToString() const
{
    std::string str = std::to_string(value);
    str.erase(str.find_last_not_of('0') + 1);
    if (!str.empty() && str.back() == '.') str.pop_back();
    return str;
}

template<>
inline std::string ValueCell<bool>::convertToString() const
{
    return value ? "TRUE" : "FALSE";
}

template<>
inline std::string ValueCell<std::string>::convertToString() const
{
    return value;
}
