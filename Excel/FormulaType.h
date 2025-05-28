#pragma once
#include <iostream>
#include <string>

struct FormulaType 
{
    static const std::string SUM;
    static const std::string AVERAGE;
    static const std::string MIN;
    static const std::string MAX;
    static const std::string COUNT;
    static const std::string LEN;
    static const std::string CONCAT;
    static const std::string SUBSTR;
};

const std::string FormulaType::SUM = "SUM";
const std::string FormulaType::AVERAGE = "AVERAGE";
const std::string FormulaType::MIN = "MIN";
const std::string FormulaType::MAX = "MAX";
const std::string FormulaType::COUNT = "COUNT";
const std::string FormulaType::LEN = "LEN";
const std::string FormulaType::CONCAT = "CONCAT";
const std::string FormulaType::SUBSTR = "SUBSTR";