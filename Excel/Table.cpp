#include "Table.h"

Table::Table(size_t r, size_t c, std::string configFile) : rows(r), cols(c)
{
    if (!config.loadFromFile(configFile)) {
        std::cerr << "Error: failed to load config from " << configFile << "\n";
        exit(1);
    }

    for (size_t i = 0; i < rows; ++i) 
    {
        container<Cell>* row = new container<Cell>();
        for (size_t j = 0; j < cols; ++j) 
        {
            row->push_back(nullptr);
        }
        matrix.push_back(row);
    }
}

Table::Table(std::string configFile)  
{  
   if (!config.loadFromFile(configFile)) {  
       std::cerr << "Error: failed to load config from " << configFile << "\n";  
       exit(1);  
   }  

   this->rows = config.getInt("initialTableRows");  
   this->cols = config.getInt("initialTableCols");

   for (size_t i = 0; i < this->rows; ++i)
   {
       container<Cell>* row = new container<Cell>();
       for (size_t j = 0; j < this->cols; ++j)
       {
           row->push_back(nullptr);
       }
       matrix.push_back(row);
   }
}

Table::Table(size_t r, size_t c, Config cfg) : rows(r), cols(c), config(cfg)
{
	for (size_t i = 0; i < rows; ++i)
	{
		container<Cell>* row = new container<Cell>();
		for (size_t j = 0; j < cols; ++j)
		{
			row->push_back(nullptr);
		}
		matrix.push_back(row);
	}
}

Table::Table(const Table& other) : rows(other.rows), cols(other.cols) 
{
    for (size_t i = 0; i < other.rows; ++i) 
    {
        container<Cell>* newRow = new container<Cell>();
        const container<Cell>* otherRow = other.matrix[i];
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
		config = other.config;

        for (size_t i = 0; i < other.rows; ++i) 
        {
            container<Cell>* newRow = new container<Cell>();
            const container<Cell>* otherRow = other.matrix[i];
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
		config = std::move(other.config);
        matrix = std::move(other.matrix);
        other.rows = 0;
        other.cols = 0;
    }
    return *this;
}

//Cell*& Table::operator[](std::string cellRef)
//{
//    if (cellRef.size() < 2 || !isalpha(cellRef[0])) 
//        throw std::invalid_argument("Invalid cell");
//
//    size_t row = cellRef[0] - 'A';
//    int col = std::stoi(cellRef.substr(1)) - 1;
//
//    if (row >= rows || col >= cols || col < 0)
//        throw std::out_of_range("Cell reference out of bounds");
//
//    return (*matrix[row])[col];
//}

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
    bool autoFit = this->config.getBool("autoFit");
    int visibleSymbols = this->config.getInt("visibleCellSymbols");

    int cellWidth = visibleSymbols;

    if (autoFit) 
    {
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

    cellWidth += 2;

    const int totalCols = cols + 1;

    auto printDivider = [&]() {
        std::cout << "|";
        for (int i = 0; i < totalCols; ++i) 
        {
            std::cout << std::string(cellWidth, '-') << "|";
        }
        std::cout << "\n";
    };

    printDivider();

    std::cout << "|" << center(" ", cellWidth) << "|";
    for (size_t j = 0; j < cols; ++j) {
        std::string num = std::to_string(j + 1);
        std::cout << center(num, cellWidth) << "|";
    }
    std::cout << "\n";

    printDivider();

    for (size_t i = 0; i < rows; ++i) 
    {
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

bool isDividerLine(const std::string& line) 
{
    for (char ch : line) 
    {
        if (ch != '|' && ch != '-' && ch != '\r' && ch != '\n')
            return false;
    }
    return true;
}

bool Table::loadTableFromFile(const std::string& filename)
{
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    std::string line;
    size_t currentRow = 0;

    while (std::getline(in, line)) 
    {
        if (isDividerLine(line))
            continue;

        if (currentRow == 0)
        {
            currentRow++;
            continue;
        }

        size_t firstPipe = line.find('|');
        size_t secondPipe = line.find('|', firstPipe + 1);

        if (secondPipe == std::string::npos)
            continue;

        container<std::string> row;
        size_t pos = secondPipe;
        size_t next;
        

        while ((next = line.find('|', pos + 1)) != std::string::npos) 
        {
            std::string value = line.substr(pos + 1, next - pos - 1);

            size_t start = value.find_first_not_of(' ');
            size_t end = value.find_last_not_of(' ');

            if (start != std::string::npos && end != std::string::npos)
                value = value.substr(start, end - start + 1);
            else
                value = "";

            row.push_back(new std::string(value));
            pos = next;
        }

        int maxRows = config.getInt("maxTableRows");
        int maxCols = config.getInt("maxTableCols");

		if (currentRow >= maxRows || row.getSize() > maxCols)
		{
			std::cerr << "Error: Exceeded maximum table size. Maximum rows: " << maxRows << ", Maximum columns: " << maxCols << "\n";
			row.free();
			return false;
		}

        for (size_t col = 0; col < row.getSize(); ++col)
        {
            std::string value = *row[col];
			common::toLower(value);

            Cell* cell = nullptr; 

            if (value.empty())
			{
				cell = new ValueCell<std::string>(value, CellType::EMPTY);
			}
			else if (value == "true" || value == "false")
            {
                bool boolValue = (value == "true");
                cell = new ValueCell<bool>(boolValue, CellType::BOOL);
            }
            else
            {
                try
                {
                    // Try to parse as number
                    size_t idx;
                    double number = std::stod(value, &idx);
                    if (idx == value.size()) // parsed whole string
                    {
                        cell = new ValueCell<double>(number, CellType::NUMBER);
                    }
                    else
                    {
                        cell = new ValueCell<std::string>(value, CellType::TEXT);
                    }
                }
                catch (...)
                {
                    cell = new ValueCell<std::string>(value, CellType::TEXT);
                }
            }

            this->setCell(currentRow, col, cell);
            delete cell;
        }

        row.free();

        ++currentRow;
    }

    return true;
}

size_t Table::countRowsFromFile(const std::string& filename)
{
	std::ifstream in(filename);
	if (!in.is_open()) return 0;

	std::string line;
	size_t rowCount = 0;

	while (std::getline(in, line))
	{
		if (isDividerLine(line))
			continue;

		if (line.empty())
			continue;

		rowCount++;
	}

	return rowCount;
}

size_t Table::countColsFromFile(const std::string& filename)
{
    std::ifstream in(filename);
    if (!in.is_open()) return 0;

    std::string line;
    size_t colCount = 0;

    if (std::getline(in, line))
    {
        size_t firstPipe = line.find('|');
        size_t secondPipe = line.find('|', firstPipe + 1);

        if (secondPipe != std::string::npos)
        {
            size_t pos = secondPipe;
            while ((pos = line.find('|', pos + 1)) != std::string::npos)
            {
                colCount++;
            }
            colCount++;
        }
    }

    return colCount;
}

Table::~Table()
{
    if (&matrix != nullptr)
    {
		matrix.free();
    } 
}