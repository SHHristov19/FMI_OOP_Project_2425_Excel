#include <iostream>
#include <string>

namespace common
{
	void clearConsole()
	{
		std::cout << "\033c"; // ANSI escape code to clear screen and reset cursor
	}

	void colorCyan()
	{
		std::cout << "\033[1;36m";
	}

	void colorBlue()
	{
		std::cout << "\033[1;34m";
	}

	void colorYellow()
	{
		std::cout << "\033[1;33m";
	}

	void colorGreen()
	{
		std::cout << "\033[1;32m";
	}

	void colorRed()
	{
		std::cout << "\033[1;31m";
	}

	void resetColor()
	{
		std::cout << "\033[0m";
	}

	void setConsoleSize()
	{
		// Set the terminal window size to width and height
		std::cout << "\033[8;40;155t";
	}

	void hideCursor()
	{
		std::cout << "\033[?25l"; // ANSI code to hide the cursor
	}

	void showCursor()
	{
		std::cout << "\033[?25h"; // ANSI code to show the cursor
	}

	// Function to move the cursor to a specific position
	void clearLine()
	{
		std::cout << "\033[A\33[2K\r" << std::flush; // ANSI escape code to move up one line and clear the line
	}

	void toLower(std::string& data)
	{
		for (char& c : data)
		{
			c = std::tolower(c);
		}
	}

	// Gets the user input in lowercase
	std::string getInput()
	{
		std::string line;

		// Hide cursor
		hideCursor();


		std::getline(std::cin, line);

		return line;
	}

	void enterValidString(std::string& data, std::string msg, bool isUpdate)
	{
		while (true)
		{ 
			std::getline(std::cin, data);

			if (isUpdate && data.empty())
			{
				return;
			}

			if (std::cin.fail() || data.empty())
			{
				displayErrorAndClearLine();
			}
			else
			{
				return;
			}
		}
	}

	void displayErrorAndClearLine()
	{
		std::cin.clear();
		common::clearLine();

		common::resetColor();
	}
}