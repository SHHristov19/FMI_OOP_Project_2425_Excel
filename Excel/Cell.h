#pragma once
#include <iostream>
#include <string>
#include "CellType.h"

// Abstract base class
class Cell {
protected:
    CellType type = CellType::EMPTY;

public:
    virtual ~Cell() = default;

    virtual std::string evaluate() const = 0;
    virtual std::string toString() const = 0;

    virtual CellType getType() const { return type; }

    virtual Cell* clone() const = 0;
};