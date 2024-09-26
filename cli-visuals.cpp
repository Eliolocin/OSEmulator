
#include <iostream>
#include "Commands.h"	
#include "Utilities.h"
#include "TypeDefs.h"

void WelcomeUser(String osname)
{
	// Used https://www.asciiart.eu/text-to-ascii-art for the ASCII art
	
	SetTextColor("magenta");
	std::cout << R"(  
+=============================================================================+
| ________  ________  ________  ________  _______   ________       ___    ___ |
||\   ____\|\   ____\|\   __  \|\   __  \|\  ___ \ |\   ____\     |\  \  /  /||
|\ \  \___|\ \  \___|\ \  \|\  \ \  \|\  \ \   __/|\ \  \___|_    \ \  \/  / /|
| \ \  \    \ \_____  \ \  \\\  \ \   ____\ \  \_|/_\ \_____  \    \ \    / / |
|  \ \  \____\|____|\  \ \  \\\  \ \  \___|\ \  \_|\ \|____|\  \    \/  /  /  |
|   \ \_______\____\_\  \ \_______\ \__\    \ \_______\____\_\  \ __/  / /    |
|    \|_______|\_________\|_______|\|__|     \|_______|\_________\\___/ /     |
|             \|_________|                            \|_________\|___|/      |
+=============================================================================+
	)";
	SetTextColor("yellow");
	std::cout << "\nHello! Welcome to the " << osname << " CLI!\n\n";
	std::cout << "Developers:\nDONATO, Adriel Joseph\nROSARIO, Elijah Nicolo\n\n";
	std::cout << "Type 'exit' to quit, 'clear' to clear the screen.\n\n";
}

void SetTextColor(String color)
{	// Lasts for whole session, until overwritten
	if (color == "red")
	{
		std::cout << "\033[1;31m";
	}
	else if (color == "green")
	{
		std::cout << "\033[1;32m";
	}
	else if (color == "yellow")
	{
		std::cout << "\033[1;33m";
	}
	else if (color == "blue")
	{
		std::cout << "\033[1;34m";
	}
	else if (color == "magenta")
	{
		std::cout << "\033[1;35m";
	}
	else if (color == "cyan")
	{
		std::cout << "\033[1;36m";
	}
	else if (color == "white")
	{
		std::cout << "\033[1;37m";
	}
	else if (color == "RESET")
	{
		std::cout << "\033[0m";
	}
	else
	{
		std::cout << "Invalid color\n";
	}
}