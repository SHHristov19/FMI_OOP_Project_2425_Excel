#include "Common.hpp"
#include "ExpressionCell.hpp"
#include "Table.h"

int main()
{
    Table* table = nullptr;

    common::clearConsole();
    common::setConsoleSize();

    std::string input;
	container<std::string> commands;
    std::string command;
    do 
    {
        input = common::getInput();
        commands = common::splitInput(input);
        common::clearLine();

        if (commands.getSize() == 2 && *commands[0] == "new")
        {
            std::string configFile = *commands[1];

            Config cfg;
            if (!cfg.loadFromFile(configFile)) 
            {
                common::clearLine();
                std::cerr << "Failed to load config on new: " << configFile << "\n";
                commands.free();
                continue;
            }

            size_t rows = cfg.getInt("initialTableRows");
            size_t cols = cfg.getInt("initialTableCols");

            table = new Table(rows, cols, configFile);
            std::cout << "Created new table " << rows << "x" << cols << " with config '" << configFile << "'\n";
        }
        else if (commands.getSize() == 3 && *commands[0] == "open")
        {
            std::string tableFile = *commands[1];
            std::string configFile = *commands[2];

            Config cfg;
            if (!cfg.loadFromFile(configFile)) 
            {
                common::clearLine();
                std::cerr << "Failed to load config on open: " << configFile << "\n";
                commands.free();
				continue;
            }

			size_t rows = table->countRowsFromFile(tableFile) - 1;
			size_t cols = table->countColsFromFile(tableFile) - 1;

            table = new Table(rows, cols, cfg);

            if (!table->loadTableFromFile(tableFile)) 
            {
                common::clearLine();
                std::cerr << "Failed to load table from '" << tableFile << "'\n";
                delete table;
                commands.free();
                continue;
            }

            std::cout << "Opened table '" << tableFile << "' with config '" << configFile << "'\n";

            table->print();

			commands.free();
        }
        else if (table != nullptr && commands.getSize() == 3 && *commands[1] == "insert")
        {
			std::string cellRef = *commands[0]; 

            if (cellRef.size() < 2 || !isalpha(cellRef[0]))
            {
                std::cerr << "Invalid cell";
            }

            size_t row = cellRef[0] - 'A';
            int col = std::stoi(cellRef.substr(1)) - 1;
            
            if (row >= table->getRowCount() || col >= table->getColCount() || col < 0)
            {
                std::cerr << "Cell reference out of bounds";
            }

			table->setCell(row, col, new ExpressionCell(*commands[2], table));

			common::clearConsole();
			table->print();
            commands.free();
        }
        else if (table != nullptr && commands.getSize() == 2 && *commands[1] == "delete")
        {
            break;
        }
        else if (table != nullptr && commands.getSize() == 2 && (*commands[1]).substr(0, 1) == "=")
        {
			// This is a cell formula
        }
        else if (input == "exit")
        {
            break;
        }
        else
        {
            common::clearLine();
            std::cerr << "Unknown command\n";
            commands.free();
        }
    } while (true);

	if (table != nullptr)
	{
		delete table;
	}

    return 0;
}