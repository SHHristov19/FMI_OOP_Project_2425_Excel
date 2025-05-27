#include <iostream>
#include "Table.h"

int main() {
    std::string tableFile = "table.txt";
    std::string configFile = "config.txt";
      
    Table table(configFile);

   /* if (!table.loadTableFromFile(tableFile)) {
        std::cerr << "Failed to load table from '" << tableFile << "'\n";
        return 1;
    }

    std::cout << "Opened table '" << tableFile << "' with config '" << configFile << "'\n";*/

    // Add a text value
    Cell* textCell = new ValueCell<std::string>("Hello", CellType::TEXT);
    table.setCell(0, 0, textCell);
    delete textCell; // Assuming setCell copies the cell

    // Add a numeric value
    Cell* numberCell = new ValueCell<double>(42.0, CellType::NUMBER);
    table.setCell(0, 1, numberCell);
    delete numberCell;

    // Add a boolean value
    Cell* boolCell = new ValueCell<bool>(true, CellType::BOOL);
    table.setCell(0, 2, boolCell);
    delete boolCell;

    // Add a formula (represented as text starting with '=')
    Cell* formulaCell = new ValueCell<std::string>("=A1+B1", CellType::FORMULA);
    table.setCell(0, 3, formulaCell);
    delete formulaCell;
     

    // 5. Печат на таблицата
    std::cout << "Table:\n";
    table.print();

    return 0;
}