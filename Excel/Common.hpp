#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "Container.hpp"

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
	std::string formatNumber(double num, int digits);
	container<std::string> splitInput(const std::string input);
	std::string fileExists(const std::string& fileName);
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

inline std::string common::formatNumber(double num, int digits)
{ 
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(digits) << num;
	return oss.str(); 
}

inline container<std::string> common::splitInput(const std::string input)
{
	container<std::string> result;
	size_t start = 0;
	size_t end = input.find(' ');

	while (end != std::string::npos)
	{
		result.push_back(new std::string(input.substr(start, end - start)));
		start = end + 1;
		end = input.find(' ', start);
	}

	result.push_back(new std::string(input.substr(start))); // Add the last part

	return result;
}

// Fnuction to try if such file withe fileName exists and can be opened
inline std::string common::fileExists(const std::string& fileName)
{
	std::fstream file(fileName);
	return file.good() ? fileName : "";
}