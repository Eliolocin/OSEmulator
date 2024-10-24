#include "MainConsole.h"
#include <iostream>
#include "Utilities.h"
#include "Commands.h"
#include "Config.h"
#include "ConsoleManager.h"

MainConsole::MainConsole() : AbsConsole("MAIN_CONSOLE") {}

void MainConsole::onEnabled()
{
	WelcomeUser("CSOPESY");
}

void MainConsole::display()
{
	//std::cout << "Main Console Display" << std::endl;
}

void MainConsole::process()
{
	String Command; // User input
	String Params; // Additional input by user

	SetTextColor("green");
	std::cout << "Enter a command: ";
	SetTextColor("white");
	std::getline(std::cin, Command);
	SetTextColor("cyan");

	if(!isInitialized())
	{
		if (Command == "initialize")
		{
			Initialize();
		}
		else if (Command == "exit")
		{
			Exit();
			ConsoleManager::getInstance()->setRunning(false); // Exits while loop, ending the program
		}
		else
		{
			SetTextColor("red");
			std::cout << "Please initialize the OS first with \"initialize\"!\n";
		}
	}else
	{
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
		else if (Command.substr(0, 10) == "screen -r ")
		{
			Params = Command.substr(10); // Extracts the process name from the command
			ScreenR(Params);
		}
		else if (Command.substr(0, 10) == "screen -s ")
		{
			Params = Command.substr(10); // Extracts the process name from the command
			ScreenS(Params);
		}
		else if (Command == "screen -ls")
		{
			ScreenLS(true);
		}
		else if (Command == "clear")
		{
			Clear();
		}
		else if (Command == "process-smi")
		{
			ProcessSmi();
		}
		else if (Command == "exit")
		{
			Exit();
			ConsoleManager::getInstance()->setRunning(false); // Exits while loop, ending the program
		}
		else
		{
			SetTextColor("red");
			std::cout << "'" << Command << "' is an invalid or unrecognized command, please try again.\n";
		}
	}
		


}