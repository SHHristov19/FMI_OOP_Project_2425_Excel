#pragma once   
#include <iostream>
#include <string>
#include "Cell.h"  
#include "Container.hpp"

class Table 
{  
private:  
	size_t rows;  
	size_t cols;
	//Config config;
	Container<Container<Cell>> matrix;

public:  
	Table() : rows(0), cols(0) {}
	Table(size_t rows, size_t cols, std::string configFile);  

	Table(const Table& other);  
	Table& operator=(const Table& other);  

	Table(Table&& other) noexcept;  
	Table& operator=(Table&& other) noexcept;  

	void setCell(size_t row, size_t col, Cell* cell);  
	Cell* getCell(size_t row, size_t col);  

	// Helper function to center the text in a cell
	std::string center(const std::string& str, int width);
	void print();  

	size_t getRowCount() const { return rows; }
	size_t getColCount() const { return cols; }
};
