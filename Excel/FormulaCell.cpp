#include "FormulaCell.h" 

Cell* FormulaCell::clone() const
{
    return new FormulaCell(*this);
}

std::string FormulaCell::evaluate() const
{
    if (rawFormula.empty() || rawFormula[0] != '=')
        return "#VALUE!";

    return evalFormula(rawFormula.substr(1));
}

std::string FormulaCell::toString() const
{
    return evaluate();
}
 
std::string FormulaCell::evalFormula(const std::string& formula) const
{
    size_t open = formula.find('(');
    size_t close = formula.find_last_of(')');

    if (open == std::string::npos || close == std::string::npos || close <= open)
        return "#VALUE!";

    std::string func = formula.substr(0, open);
    std::string argsStr = formula.substr(open + 1, close - open - 1);

    container<std::string> args;
    size_t pos = 0, next;

    while ((next = argsStr.find(',', pos)) != std::string::npos)
    {
        std::string arg = argsStr.substr(pos, next - pos);
        args.push_back(new std::string(arg));
        pos = next + 1;
    }
    args.push_back(new std::string(argsStr.substr(pos)));

    for (size_t i = 0; i < args.getSize(); ++i)
    {
        std::string& a = *args[i];
        size_t s = a.find_first_not_of(' ');
        size_t e = a.find_last_not_of(' ');

        if (s != std::string::npos && e != std::string::npos)
            a = a.substr(s, e - s + 1);
        else
            a = "";
    }

    std::string result = evalFunction(func, args);

    args.free();

    return result;
}

std::string FormulaCell::evalFunction(const std::string& name, const container<std::string>& args) const
{
    if (name == FormulaType::SUM || name == FormulaType::AVERAGE ||
        name == FormulaType::MIN || name == FormulaType::MAX || name == FormulaType::COUNT)
    {

        if (args.getSize() < 1 || args.getSize() > 255)
			return "#VALUE!";

		if (name == FormulaType::SUM && args.getSize() < 1) 
			return "#VALUE!";

		if (name == FormulaType::AVERAGE && args.getSize() < 2)
			return "#VALUE!";

        if (name == FormulaType::COUNT && args.getSize() != 1 && !isRange(*args[0]))
			return "#VALUE!";

        if (name == FormulaType::MIN && args.getSize() != 1)
			return "#VALUE!";

        if (name == FormulaType::MAX && args.getSize() != 1)
            return "#VALUE!";

        double result = 0;
        double count = 0;
        bool found = false;
        double minVal = DBL_MAX, maxVal = -DBL_MAX;

        for (size_t i = 0; i < args.getSize(); ++i)
        {
            const std::string& p = *args[i];
            if (isRange(p))
            {
                container<std::string> refs = extractRange(p);
                for (size_t j = 0; j < refs.getSize(); ++j) 
                {
                    std::string val = getCellValue(*refs[j]);
                    if (val == "#VALUE!")
                        return "#VALUE!";
                    if (isNumber(val)) 
                    {
                        double num = std::stod(val);
                        result += num;
                        count++;
                        if (num < minVal) minVal = num;
                        if (num > maxVal) maxVal = num;
                        found = true;
                    }
                    else if (name == FormulaType::COUNT && !val.empty())
                    {
                        result++;
                        found = true;
                    }
                }
                refs.free();
            }
            else if (isCellRef(p))
            {
                std::string val = getCellValue(p);
                if (val == "#VALUE!")
                    return "#VALUE!";

                if (isNumber(val)) 
                {
                    double num = std::stod(val);
                    result += num;
                    count++;
                    if (num < minVal) minVal = num;
                    if (num > maxVal) maxVal = num;
                    found = true;
                }
                else if (name == FormulaType::COUNT && !val.empty())
                {
                    result++;
                    found = true;
                }
            }
            else if (isNumber(p))
            {
                double num = std::stod(p);
                result += num;
                count++;
                if (num < minVal) minVal = num;
                if (num > maxVal) maxVal = num;
                found = true;
            }
            else if (isBool(p))
            {
                result += (p == "TRUE" ? 1 : 0);
                count++;
                found = true;
            }
        }

        if (!found) return "#VALUE!";

        if (name == FormulaType::SUM) return common::formatNumber(result, 2);
        if (name == FormulaType::AVERAGE) return common::formatNumber(result / count, 2);
        if (name == FormulaType::MIN) return common::formatNumber(minVal, 2);
        if (name == FormulaType::MAX) return common::formatNumber(maxVal, 2);
        if (name == FormulaType::COUNT) return common::formatNumber(count, 0);
    }

    if (name == FormulaType::LEN)
    {
        if (args.getSize() != 1 || isRange(*args[0]))
            return "#VALUE!";
        return std::to_string((*args[0]).size());
    }

    if (name == FormulaType::CONCAT)
    {
        if (args.getSize() != 2 || !isRange(*args[0]))
            return "#VALUE!";
        container<std::string> refs = extractRange(*args[0]);
        std::string delimiter = *args[1];
        delimiter.erase(std::remove(delimiter.begin(), delimiter.end(), '\"'), delimiter.end());
        std::string result = "";
        bool first = true;
        for (size_t i = 0; i < refs.getSize(); ++i)
        {
            std::string val = getCellValue(*refs[i]);
            if (!val.empty() && val != "#VALUE!")
            {
                if (!first) result += delimiter;
                result += val;
                first = false;
            }
        }
        refs.free();
        return result.empty() ? "#VALUE!" : result;
    }

    if (name == FormulaType::SUBSTR)
    {
        if (args.getSize() != 3 || !isNumber(*args[1]) || !isNumber(*args[2]))
            return "#VALUE!";

        std::string base = *args[0];
        int start = std::stoi(*args[1]);
        int length = std::stoi(*args[2]);

        if (start < 0 || length <= 0 || static_cast<size_t>(start) >= base.size() || static_cast<size_t>(start + length) > base.size())
            return "#VALUE!";

        return base.substr(start, length);
    }

    return "#VALUE!";
}

bool FormulaCell::isCellRef(const std::string& s) const
{
    return s.size() >= 2 && isalpha(s[0]) && isdigit(s[1]);
}

bool FormulaCell::isRange(const std::string& s) const
{
    return s.find(':') != std::string::npos;
}

bool FormulaCell::isNumber(const std::string& s) const
{
    if (s.empty()) return false;
    for (char c : s) if (!isdigit(c) && c != '.' && c != '-') return false;
    return true;
}

bool FormulaCell::isBool(const std::string& s) const
{
    return s == "TRUE" || s == "FALSE";
}

std::string FormulaCell::getCellValue(const std::string& ref) const
{
    if (!isCellRef(ref)) return "#VALUE!";
    size_t row = ref[0] - 'A';
    size_t col = std::stoi(ref.substr(1)) - 1;

    Cell* cell = parentTable->getCell(row, col);
    if (!cell) return "0";
    std::string val = cell->evaluate();
    return val;
}

container<std::string> FormulaCell::extractRange(const std::string& range) const
{
    container<std::string> result;

    size_t sep = range.find(':');

    if (sep == std::string::npos)
        return result;

    std::string start = range.substr(0, sep);
    std::string end = range.substr(sep + 1);

    size_t row1 = start[0] - 'A';
    size_t col1 = std::stoi(start.substr(1)) - 1;
    size_t row2 = end[0] - 'A';
    size_t col2 = std::stoi(end.substr(1)) - 1;

    if (row1 > row2) std::swap(row1, row2);
    if (col1 > col2) std::swap(col1, col2);

    for (size_t i = row1; i <= row2; ++i)
    {
        for (size_t j = col1; j <= col2; ++j)
        {
            char rowChar = 'A' + i;
            std::string* ref = new std::string(rowChar + std::to_string(j + 1));
            result.push_back(ref);
        }
    }

    return result;
}