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

        // new tableFile configFile
		if (commands.getSize() == 3 && *commands[0] == "new") 
        {
            std::string configFile = common::fileExists(*commands[2]);
            std::string tabelFile = common::fileExists(*commands[1]);

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
            table->setTableFileName(tabelFile);
            table->setConfigFileName(configFile);

            std::cout << "Created new table " << rows << "x" << cols << " with config '" << configFile << "'\n";
            table->print(std::cout);
            commands.free();
        }
        // open tableFile configFile
		else if (commands.getSize() == 3 && *commands[0] == "open") 
        {
            std::string tableFile = common::fileExists(*commands[1]);
            std::string configFile = common::fileExists(*commands[2]);

            Config cfg;
            if (configFile.empty() || !cfg.loadFromFile(configFile))
            {
                common::clearLine();
                std::cerr << "Failed to load config on open: " << configFile << "\n";
                commands.free();
				continue;
            }
            if (tableFile.empty())
            {
                common::clearLine();
                std::cerr << "Table file name not exists.\n";
                commands.free();
				continue;
            }

			size_t rows = table->countRowsFromFile(tableFile) - 1;
			size_t cols = table->countColsFromFile(tableFile) - 1;
            clearConsole = cfg.getBool("clearConsoleAfterCommand");

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
        // A1 insert ...
        else if (table != nullptr && commands.getSize() == 3 && *commands[1] == "insert") 
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

			table->setCell(row, col, *commands[2]);

			if (clearConsole)common::clearConsole();
			table->print(std::cout);
            commands.free();
			std::ofstream out(table->getTableFileName());
			table->print(out);
        }
        // A1=A2  // B12=A11
        else if (table != nullptr 
            && commands.getSize() == 1 
			&& commands[0]->find_first_of("=") >= 2 // Ensure the string has at least 2 characters before '='
			&& commands[0]->size() - commands[0]->find_last_of("=") >= 2 // Ensure the string has at least 2 characters after '='
			&& std::count((*commands[0]).begin(), (*commands[0]).end(), '=') == 1) // Find if the string contains exactly one '='
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
                table->setCell(row, col, *commands[0]);  
                if (clearConsole) common::clearConsole();
                table->print(std::cout);
                commands.free();
            }
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
        else if (table != nullptr && commands.getSize() == 2 && commands[1]->substr(0, 1) == "=") // A1 =FORMULA
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
                table->setCell(row, col, *commands[1]);
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