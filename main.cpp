// OSEmulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


using namespace std;
#include <iostream>
#include "ConsoleManager.h"
#include "Commands.h" // Header file containing ALL console command declarations
#include "Utilities.h" // Header file containing ALL utility function declarations

int main()
{
	string OSName = "CSOPESY";
	string Command;
    bool running = true; // Program keeps running while True
    
	// WelcomeUser(OSName); // Prints welcome message including Header ASCII and OS Name
	ConsoleManager::initialize(); // Initialize ConsoleManager Singleton

	while (running)
	{
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();

		running = ConsoleManager::getInstance()->isRunning();
	}

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

/*
SetTextColor("green");
std::cout << "Enter a command: ";
SetTextColor("white");
std::cin >> Command;
SetTextColor("cyan");

if (Command == "initialize")
{
	Initialize();
}
else if (Command == "report-util")
{
	ReportUtil();
}
else if (Command == "scheduler-stop")
{
	SchedulerStop();
}
else if (Command == "scheduler-test")
{
	SchedulerTest();
}
else if (Command == "screen")
{
	Screen();
}
else if (Command == "clear")
{
	Clear();
}
else if (Command == "exit")
{
	Exit();
	Running = false; // Exits while loop, ending the program
}
else
{
	SetTextColor("red");
	std::cout << "'" << Command << "' is an invalid or unrecognized command, please try again.\n";
}
*/