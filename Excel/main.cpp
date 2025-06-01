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
	bool clearConsole = true;
    do 
    {
        input = common::getInput();
        commands = common::splitInput(input);

		if (commands.getSize() == 2 && *commands[0] == "new") // new configFile
        {
            std::string configFile = *commands[1];

            Config cfg;
			if (table != nullptr)
			{
				delete table;
				table = nullptr;
			}
			if (configFile.empty())
			{
				common::clearLine();
				std::cerr << "Config file name cannot be empty\n";
				commands.free();
				continue;
			}

			cfg.createConfigFile(configFile); // Create a new config file if it doesn't exist

            if (!cfg.loadFromFile(configFile)) 
            {
                common::clearLine();
                std::cerr << "Failed to load config on new: " << configFile << "\n";
                commands.free();
                continue;
            }

            size_t rows = cfg.getInt("initialTableRows");
            size_t cols = cfg.getInt("initialTableCols");
			clearConsole = cfg.getBool("clearConsoleAfterCommand"); // Check if we should clear the console after commands

            table = new Table(rows, cols, configFile);
            std::cout << "Created new table " << rows << "x" << cols << " with config '" << configFile << "'\n";
        }
		else if (commands.getSize() == 3 && *commands[0] == "open") // open tableFile configFile
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

            table->setTableFileName(tableFile);
			table->setConfigFileName(configFile);

            table->print(std::cout);
			commands.free();
        }
        else if (table != nullptr && commands.getSize() == 3 && *commands[1] == "insert") // A1 insert ...
        {
			std::string cellRef = *commands[0]; 

            if (cellRef.size() < 2 || !isalpha(cellRef[0]))
            {
                std::cerr << "Invalid cell\n";
            }

            size_t row = cellRef[0] - 'A';
            size_t col = std::stoi(cellRef.substr(1)) - 1;
            
            if (row >= table->getRowCount() || col >= table->getColCount() || col < 0)
            {
                std::cerr << "Cell reference out of bounds\n";
            }

			table->setCell(row, col, new ExpressionCell(*commands[2], table));

			if (clearConsole)common::clearConsole();
			table->print(std::cout);
            commands.free();
			std::ofstream out(table->getTableFileName());
			table->print(out);
        }
        else if (table != nullptr && commands.getSize() == 2 && *commands[1] == "delete") // A1 delete
        {
            std::string cellRef = *commands[0];
            size_t row = cellRef[0] - 'A';
            size_t col = std::stoi(cellRef.substr(1)) - 1;

			if (row >= table->getRowCount() || col >= table->getColCount() || col < 0)
			{
				std::cerr << "Cell reference out of bounds\n";
			}
			else
			{
				table->setCell(row, col, nullptr);
				if (clearConsole) common::clearConsole();
				table->print(std::cout);
				commands.free();
                std::ofstream out(table->getTableFileName());
                table->print(out);
			}
        }
        else if (table != nullptr && commands.getSize() == 2 && (*commands[1]).substr(0, 1) == "=") // A1 =FORMULA
        {
            std::string cellRef = *commands[0];
            size_t row = cellRef[0] - 'A';
            size_t col = std::stoi(cellRef.substr(1)) - 1;

            if (row >= table->getRowCount() || col >= table->getColCount() || col < 0)
            {
                std::cerr << "Cell reference out of bounds\n";
            }
            else
            {
                table->setCell(row, col, new ExpressionCell(*commands[1], table));
                if (clearConsole) common::clearConsole();
                table->print(std::cout);
                commands.free();
                std::ofstream out(table->getTableFileName());
                table->print(out);
            }
        }
        // To do A1=A2
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