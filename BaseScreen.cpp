#include "BaseScreen.h"
#include <iostream>
#include "Process.h"
#include "AbsConsole.h"
#include "Utilities.h"
#include "Commands.h"
#include "ConsoleManager.h"

// Screens drawn by the "screen-r or screen-s" commands
BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : attachedProcess(process), AbsConsole(processName) {}

void BaseScreen::onEnabled()
{
	printProcessInfo();
}

void BaseScreen::display()
{

}

std::shared_ptr<Process> BaseScreen::getProcess() // Returns process attached to this BaseScreen
{
	return attachedProcess;
}

void BaseScreen::process()
{
	String Command; // User input
	String Params; // Additional input by user

	SetTextColor("green");
	std::cout << "Enter a command: ";
	SetTextColor("white");
	std::getline(std::cin, Command);
	SetTextColor("cyan");

	if (Command == "exit")
	{
		ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);
	}
	else if (Command == "execute") // Test function that executes all of processes' commands manually using 'Main' thread
	{
		for (int i = 0; i < attachedProcess->getTotalCommandCounter(); i++)
		{
			attachedProcess->executeCurrentCommand();
			attachedProcess->moveToNextLine();
		}
	}
	else
	{
		SetTextColor("red");
		std::cout << "'" << Command << "' is an invalid or unrecognized command, please try again.\n";
	}
}

void BaseScreen::printProcessInfo() const
{
	SetTextColor("white");
	std::cout << "Process Name: " << attachedProcess->getName() << std::endl;
	std::cout << "Process ID: " << attachedProcess->getPid() << std::endl << std::endl;

	std::cout << "Current Line of Instruction: " << attachedProcess->getCommandCounter() << std::endl;
	std::cout << "Total Lines of Instruction: " << (attachedProcess->getTotalCommandCounter()) << std::endl << std::endl;

	std::cout << "Time Started: " << convertTime(attachedProcess->getTimeStarted()) << std::endl;
	std::cout << "Time Finished: " << convertTime(attachedProcess->getTimeFinished()) << std::endl << std::endl;
	SetTextColor("yellow");
	std::cout << "Type 'exit' to return to the main menu." << std::endl << std::endl;
	//std::cout << "Time Finished: " << convertTime(attachedProcess->getTimeFinished()) << std::endl;
}
