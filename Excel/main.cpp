#include <iostream>
#include "Table.h"
#include "FormulaCell.hpp"

int main() {
    std::string tableFile = "table.txt";
    std::string configFile = "config.txt";
      
    Table table(configFile);

   /* if (!table.loadTableFromFile(tableFile)) {
        std::cerr << "Failed to load table from '" << tableFile << "'\n";
        return 1;
    }

    std::cout << "Opened table '" << tableFile << "' with config '" << configFile << "'\n";*/

    table.setCell(0, 0, new ValueCell<std::string>("5", CellType::TEXT));   // A1
    table.setCell(0, 1, new ValueCell<std::string>("10", CellType::TEXT));  // A2
    table.setCell(0, 2, new ValueCell<std::string>("15", CellType::TEXT));  // A3

    std::cout << "Table:\n";
    table.print();

     
    FormulaCell* sumFormula = new FormulaCell("=SUM(A1:A3)", &table);
    table.setCell(0, 3, sumFormula); // A4
    delete sumFormula;

	FormulaCell* avgFormula = new FormulaCell("=AVERAGE(A1:A4)", &table);
	table.setCell(0, 4, avgFormula);

    std::cout << "Table:\n";
    table.print();

    return 0;
}