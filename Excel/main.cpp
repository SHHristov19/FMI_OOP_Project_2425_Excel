#include <iostream>
#include "Config.h"
#include "Table.h"
#include "ValueCell.hpp"

int main() {
    

    Config config;

    // 1. Зареждане на конфигурация
    if (!config.loadFromFile("config.txt")) {
        std::cerr << "Error: Failed to load configuration.\n";
        return 1;
    }

    config.print(); // по избор: показва конфигурацията

    // 2. Извличане на размери от конфигурацията
    int rows = config.getInt("initialTableRows");
    int cols = config.getInt("initialTableCols");

    std::cout << "\nCreating table with size " << rows << "x" << cols << "\n\n";

    // 3. Създаване на таблицата
    Table table(rows, cols, "");
    //exit(0);
    // 4. Добавяне на примерни клетки
    table.setCell(0, 0, new ValueCell<int>(123, CellType::NUMBER));               // A1
    table.setCell(0, 1, new ValueCell<std::string>("Hello", CellType::TEXT));     // B1
    table.setCell(1, 0, new ValueCell<bool>(true, CellType::BOOL));               // A2
    table.setCell(1, 1, new ValueCell<double>(3.14, CellType::NUMBER));           // B2

    // 5. Печат на таблицата
    std::cout << "Table:\n";
    table.print();

    return 0;
}
