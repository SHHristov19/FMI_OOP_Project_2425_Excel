#pragma once
#include <iostream>
#include "CellType.h"

// Abstract base class
class Cell 
{
protected:
    CellType type = CellType::EMPTY;
    std::string rowValue;

public:
    virtual ~Cell() = default;

    virtual std::string evaluate() const = 0;
    virtual std::string toString() const = 0;

    virtual CellType getType() const { return this->type; }

    virtual Cell* clone() const = 0;

    void setRowValue(const std::string& value) { this->rowValue = value; }
    std::string getRowValue() const { return this->rowValue; }
};