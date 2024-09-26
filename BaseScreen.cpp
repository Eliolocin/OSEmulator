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

	std::cout << "Current Line of Instruction: " << attachedProcess->getCurrentLine() << std::endl;
	std::cout << "Total Lines of Instruction: " << attachedProcess->getTotalLines() << std::endl << std::endl;

	std::cout << "Time Started: " << convertTime(attachedProcess->getTimeStarted()) << std::endl << std::endl;
	//std::cout << "Time Finished: " << convertTime(attachedProcess->getTimeFinished()) << std::endl << std::endl;
	SetTextColor("yellow");
	std::cout << "Type 'exit' to return to the main menu." << std::endl << std::endl;
	//std::cout << "Time Finished: " << convertTime(attachedProcess->getTimeFinished()) << std::endl;
}
