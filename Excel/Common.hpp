#pragma once
#include <iostream>
#include <string>

namespace common
{
	void clearConsole();
	void setConsoleSize();
	void hideCursor();
	void showCursor();
	void clearLine();
	void toLower(std::string& data);
	std::string getInput();
	void displayErrorAndClearLine();
	void enterValidString(std::string& data, std::string msg, bool isUpdate);

}

inline void common::clearConsole()
{
	std::cout << "\033c"; // ANSI escape code to clear screen and reset cursor
}

inline void common::setConsoleSize()
{
	// Set the terminal window size to width and height
	std::cout << "\033[8;40;155t";
}

inline void common::hideCursor()
{
	std::cout << "\033[?25l"; // ANSI code to hide the cursor
}

inline void common::showCursor()
{
	std::cout << "\033[?25h"; // ANSI code to show the cursor
}

// Function to move the cursor to a specific position
inline void common::clearLine()
{
	std::cout << "\033[A\33[2K\r" << std::flush; // ANSI escape code to move up one line and clear the line
}

inline void common::toLower(std::string& data)
{
	for (char& c : data)
	{
		c = std::tolower(c);
	}
}

// Gets the user input in lowercase
inline std::string common::getInput()
{
	std::string line;

	// Hide cursor
	common::hideCursor();


	std::getline(std::cin, line);

	return line;
}

inline void common::displayErrorAndClearLine()
{
	std::cin.clear();
	common::clearLine();
}

inline void common::enterValidString(std::string& data, std::string msg, bool isUpdate)
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
			common::displayErrorAndClearLine();
		}
		else
		{
			return;
		}
	}
} 