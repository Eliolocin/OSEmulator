#include <iostream>
#include "Commands.h"	
#include "Utilities.h"
#include "Screen.h"
#include "TypeDefs.h"
#include "Process.h"
#include "ConsoleManager.h"
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>

void generateHundredPrints(int numOfProcesses)
{
	for (int i = 0; i < numOfProcesses; i++)
	{
		String processName = "process_" + std::to_string(i);
		Process newProcess = Process(processName, i); // PID Naming Convention: Number of Consoles/Processes present+1
		newProcess.populatePrintCommands(100); // Add dummy Print commands
		BaseScreen newScreen = BaseScreen(std::make_shared<Process>(newProcess), processName);

		ConsoleManager::getInstance()->registerScreen(std::make_shared<BaseScreen>(newScreen));
		std::cout << "Process \"" << processName << "\" generated w/ 100 print commands." << std::endl;
	}
	std::cout << "\n";
}