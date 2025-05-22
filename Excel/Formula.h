#pragma once
#include <string>
#include "Table.h"

class IFormula {
public:
    virtual ~IFormula() = default;
    virtual std::string evaluate(Table* table) const = 0;
    virtual IFormula* clone() const = 0;
};
