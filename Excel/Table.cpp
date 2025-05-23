#include "Table.h"
#include "Config.h"

// Инициализира празна матрица с посочени редове и колони
Table::Table(size_t r, size_t c, std::string config) : rows(r), cols(c) 
{
    for (size_t i = 0; i < rows; ++i) 
    {
        Container<Cell>* row = new Container<Cell>();
        for (size_t j = 0; j < cols; ++j) 
        {
            row->push_back(nullptr);
        }
        matrix.push_back(row);
    }
}

// Копиращ конструктор
Table::Table(const Table& other) : rows(other.rows), cols(other.cols) 
{
    for (size_t i = 0; i < other.rows; ++i) 
    {
        Container<Cell>* newRow = new Container<Cell>();
        const Container<Cell>* otherRow = other.matrix[i];
        for (size_t j = 0; j < other.cols; ++j) 
        {
            Cell* copied = nullptr;
            if ((*otherRow)[j])
            {
                copied = (*otherRow)[j]->clone();
            }
            newRow->push_back(copied);
        }
        matrix.push_back(newRow);
    }
}

Table& Table::operator=(const Table& other) 
{
    if (this != &other) 
    {
        this->~Table();
        rows = other.rows;
        cols = other.cols;
        for (size_t i = 0; i < other.rows; ++i) 
        {
            Container<Cell>* newRow = new Container<Cell>();
            const Container<Cell>* otherRow = other.matrix[i];
            for (size_t j = 0; j < other.cols; ++j) 
            {
                Cell* copied = nullptr;
                if ((*otherRow)[j])
                {
                    copied = (*otherRow)[j]->clone();
                }
                newRow->push_back(copied);
            }
            matrix.push_back(newRow);
        }
    }
    return *this;
}

Table::Table(Table&& other) noexcept : rows(other.rows), cols(other.cols), matrix(std::move(other.matrix)) 
{
    other.rows = 0;
    other.cols = 0;
}

Table& Table::operator=(Table&& other) noexcept 
{
    if (this != &other)
    {
        this->~Table();
        rows = other.rows;
        cols = other.cols;
        matrix = std::move(other.matrix);
        other.rows = 0;
        other.cols = 0;
    }
    return *this;
}

void Table::setCell(size_t row, size_t col, Cell* cell) 
{  
    if (row >= rows || col >= cols) return;

    if ((*matrix[row])[col])
    {
        delete (*matrix[row])[col];
    }
    ((*matrix[row]))[col] = cell ? cell->clone() : nullptr;
}

Cell* Table::getCell(size_t row, size_t col) 
{
    if (row >= rows || col >= cols) return nullptr;
    return (*matrix[row])[col];
}

std::string Table::center(const std::string& str, int width)
{
    int len = str.size();
    if (len >= width) return str.substr(0, width);

    int totalPadding = width - len;
    int left = (totalPadding + 1) / 2;
    int right = totalPadding / 2;

    return std::string(left, ' ') + str + std::string(right, ' ');
}

void Table::print() 
{
    Config config;
    config.loadFromFile("config.txt");

    bool autoFit = config.getBool("autoFit");
    int visibleSymbols = config.getInt("visibleCellSymbols");

    // Определяме ширината на всяка клетка
    int cellWidth = visibleSymbols;

    if (autoFit) 
    {
        // Намираме най-дългата стойност в таблицата
        for (size_t i = 0; i < rows; ++i) 
        {
            for (size_t j = 0; j < cols; ++j) 
            {
                const Cell* cell = (*matrix[i])[j];
                if (cell) 
                {
                    int len = cell->evaluate().length();
                    if (len > cellWidth) 
                    {
                        cellWidth = len;
                    }
                }
            }
        }
    }

    cellWidth += 2; // 1 интервал от всяка страна

    const int totalCols = cols + 1;

    auto printDivider = [&]() {
        std::cout << "|";
        for (int i = 0; i < totalCols; ++i) 
        {
            std::cout << std::string(cellWidth, '-') << "|";
        }
        std::cout << "\n";
    };

    // Горен разделител
    printDivider();

    // Заглавен ред с номера на колоните
    std::cout << "|" << center(" ", cellWidth) << "|";
    for (size_t j = 0; j < cols; ++j) {
        std::string num = std::to_string(j + 1);
        std::cout << center(num, cellWidth) << "|";
    }
    std::cout << "\n";

    // Втори разделител
    printDivider();

    // Всички редове
    for (size_t i = 0; i < rows; ++i) {
        char rowChar = 'A' + i;
        std::cout << "|" << center(std::string(1, rowChar), cellWidth) << "|";
        for (size_t j = 0; j < cols; ++j) {
            const Cell* cell = (*matrix[i])[j];
            std::string val = cell ? cell->evaluate() : " ";
            std::cout << center(val, cellWidth) << "|";
        }
        std::cout << "\n";
        printDivider();
    }
}
