using namespace std;
#include <iostream>
#include "Commands.h"	
#include "Utilities.h"

void WelcomeUser(string osname)
{
	// Used https://www.asciiart.eu/text-to-ascii-art for the ASCII art
	
	SetTextColor("magenta");
	cout << R"(  
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
	cout << "\nHello! Welcome to the " << osname << " CLI!\n";
	cout << "Type 'exit' to quit, 'clear' to clear the screen.\n\n";
}

void SetTextColor(string color)
{	// Lasts for whole session, until overwritten
	if (color == "red")
	{
		cout << "\033[1;31m";
	}
	else if (color == "green")
	{
		cout << "\033[1;32m";
	}
	else if (color == "yellow")
	{
		cout << "\033[1;33m";
	}
	else if (color == "blue")
	{
		cout << "\033[1;34m";
	}
	else if (color == "magenta")
	{
		cout << "\033[1;35m";
	}
	else if (color == "cyan")
	{
		cout << "\033[1;36m";
	}
	else if (color == "white")
	{
		cout << "\033[1;37m";
	}
	else if (color == "RESET")
	{
		cout << "\033[0m";
	}
	else
	{
		cout << "Invalid color\n";
	}
}