#include "Table.h" 
#include "FactoryCell.hpp"

Table::Table(size_t r, size_t c, std::string configFile) : rows(r), cols(c)
{
    if (!config.loadFromFile(configFile)) 
    {
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
        table.push_back(row);
    }
}

Table::Table(std::string configFile)  
{  
   if (!config.loadFromFile(configFile)) 
   {  
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
       table.push_back(row);
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
		table.push_back(row);
	}
}

Table::Table(const Table& other) : rows(other.rows), cols(other.cols) 
{
    for (size_t i = 0; i < other.rows; ++i) 
    {
        container<Cell>* newRow = new container<Cell>();
        const container<Cell>* otherRow = other.table[i];
        for (size_t j = 0; j < other.cols; ++j) 
        {
            Cell* copied = nullptr;
            if ((*otherRow)[j])
            {
                copied = (*otherRow)[j]->clone();
            }
            newRow->push_back(copied);
        }
        table.push_back(newRow);
    }
}

//Table& Table::operator=(const Table& other) 
//{
//    if (this != &other) 
//    {
//        this->~Table();
//        rows = other.rows;
//        cols = other.cols;
//		config = other.config;
//		table.free();
//
//        for (size_t i = 0; i < other.rows; ++i) 
//        {
//            container<Cell>* newRow = new container<Cell>();
//            const container<Cell>* otherRow = other.table[i];
//            for (size_t j = 0; j < other.cols; ++j) 
//            {
//                Cell* copied = nullptr;
//                if ((*otherRow)[j])
//                {
//                    copied = (*otherRow)[j]->clone();
//                }
//                newRow->push_back(copied);
//            }
//            table.push_back(newRow);
//        }
//    }
//    return *this;
//}

Table::Table(Table&& other) noexcept : rows(other.rows), cols(other.cols), table(std::move(other.table)) 
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
        table = std::move(other.table);

		configFileName = other.configFileName;
		tableFileName = other.tableFileName;
		tableRowFileName = other.tableRowFileName;

		other.configFileName.clear();
		other.tableFileName.clear();
		other.tableRowFileName.clear();

		other.table.free();
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

void Table::setCell(size_t row, size_t col, std::string data)
{  
    if (row >= rows || col >= cols) return;

    Cell* factoryCell = new FactoryCell(data, this);

    if ((*table[row])[col])
    {
        delete (*table[row])[col];
    }
    
    ((*table[row]))[col] = factoryCell ? factoryCell->clone() : nullptr;

	if (factoryCell != nullptr) delete factoryCell;
}

Cell* Table::getCell(size_t row, size_t col) 
{
    if (row >= rows || col >= cols) return nullptr;
    return (*table[row])[col];
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

void Table::printDivider(int cellWidth, std::ostream& os) const
{
	os << "|";
    for (int i = 0; i < cols + 1; ++i) 
    {
        os << std::string(cellWidth, '-') << "|";
    }
    os << "\n";
}

void Table::print(std::ostream& os)
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
                Cell* cell = (*table[i])[j];
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

    printDivider(cellWidth, os);

    os << "|" << center(" ", cellWidth) << "|";
    for (size_t j = 0; j < cols; ++j) {
        std::string num = std::to_string(j + 1);
        os << center(num, cellWidth) << "|";
    }
    os << "\n";

    printDivider(cellWidth, os);

    for (size_t i = 0; i < rows; ++i) 
    {
        char rowChar = 'A' + i;
        os << "|" << center(std::string(1, rowChar), cellWidth) << "|";
        for (size_t j = 0; j < cols; ++j) 
        {
            Cell* cell = (*table[i])[j];
            std::string val = cell ? cell->evaluate() : " ";
            os << center(val, cellWidth) << "|";
        }
        os << "\n";
        printDivider(cellWidth, os);
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

bool Table::loadTableFromFile()
{
    std::ifstream in(this->tableRowFileName);
    if (!in.is_open()) return false;

    std::string line;
    while (std::getline(in, line)) 
    {
        size_t firstSpace = line.find(' ');
        if (firstSpace == std::string::npos)
            continue; // skip invalid lines
        std::string token1 = line.substr(0, firstSpace);

        size_t start = line.find_first_not_of(" ", firstSpace);
        if(start == std::string::npos)
            continue; // no second token
        size_t secondSpace = line.find(' ', start);

        std::string token2;
        std::string rawInput;
        if (secondSpace == std::string::npos) 
        {
            token2 = line.substr(start);
            rawInput = "";
        }
        else 
        {
            token2 = line.substr(start, secondSpace - start);
            rawInput = line.substr(secondSpace + 1);
            size_t nonSpace = rawInput.find_first_not_of(" ");

            if (nonSpace != std::string::npos)
                rawInput = rawInput.substr(nonSpace);
            else
                rawInput = "";
        }

        size_t row = std::stoul(token1);
        size_t col = std::stoul(token2);
        this->setCell(row, col, rawInput);
    }

    return true;
}

bool Table::saveTable() 
{ 
    std::ofstream printOut(this->tableFileName);
    this->print(printOut);

    std::ofstream out(this->tableRowFileName);
    if (!out.is_open()) return false;

    for (size_t row = 0; row < rows; ++row) 
    {
        for (size_t col = 0; col < cols; ++col) 
        {
            Cell* cell = (*table[row])[col];
            if (cell && cell->getType() != CellType::EMPTY)
            {
                out << row << " " << col << " " << cell->getRowValue() << "\n";
            }
        }
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