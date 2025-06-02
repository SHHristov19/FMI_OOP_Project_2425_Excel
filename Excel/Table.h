#pragma once
#include "Config.h"
#include "ValueCell.hpp"
#include "Container.hpp"
#include "Common.hpp"

class Table 
{  
private:  
	size_t rows;  
	size_t cols;
	Config config;
	container<container<Cell>> matrix;
	std::string tableFileName;
	std::string configFileName;

	void printDivider(int cellWidth, std::ostream& os) const;
	// Helper function to center the text in a cell
	std::string center(const std::string& str, int width);
public:  
	Table() : rows(0), cols(0), config(Config()) {}
	Table(std::string configFile);
	Table(size_t rows, size_t cols, std::string configFile);  
	Table(size_t rows, size_t cols, Config config);

	~Table();
	Table(const Table& other);  
	Table& operator=(const Table& other);  

	Table(Table&& other) noexcept;  
	Table& operator=(Table&& other) noexcept;  
	//Cell*& operator[](std::string cellRef);

	void setCell(size_t row, size_t col, std::string data);  
	Cell* getCell(size_t row, size_t col);

	
	void print(std::ostream& os);

	size_t getRowCount() const { return rows; }
	size_t getColCount() const { return cols; }

	void setTableFileName(const std::string& fileName) { tableFileName = fileName; }
	void setConfigFileName(const std::string& fileName) { configFileName = fileName; }
	std::string getTableFileName() const { return tableFileName; }
	std::string getConfigFileName() const { return configFileName; }

	bool loadTableFromFile(const std::string& filename);

	size_t countRowsFromFile(const std::string& filename);
	size_t countColsFromFile(const std::string& filename);
};
