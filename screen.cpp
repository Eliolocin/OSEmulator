
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
#include "Config.h"



void ScreenR(String processName)
{
	ConsoleManager::getInstance()->switchConsole(processName);
}

void ScreenS(String processName)
{
	auto now = std::chrono::system_clock::now();

	// Convert the time_point to a time_t, which represents the time in seconds since the epoch
	//time_t startTime = std::chrono::system_clock::to_time_t(now);
	//time_t endTime = 0;
	// Convert the time_t to a tm structure
	//std::tm* startTime = std::localtime(&currentTime);
	//std::tm* endTime = 0;

	int consolesPresent = (ConsoleManager::getInstance()->getConsoleTableSize() - 1); // Exclude main console from count

	Process newProcess = Process(processName, consolesPresent); // PID Naming Convention: Number of Consoles/Processes present+1
	newProcess.populatePrintCommands(100); // Add dummy Print commands
	BaseScreen newScreen = BaseScreen(std::make_shared<Process>(newProcess), processName);

	ConsoleManager::getInstance()->registerScreen(std::make_shared<BaseScreen>(newScreen));
	//ConsoleManager::getInstance()->switchConsole(processName);
}

String ScreenLS(bool printToConsole)
{
	// Get all processes through their console
	std::vector<std::shared_ptr<BaseScreen>> processScreenList = ConsoleManager::getInstance()->getAllProcessScreens();

	// Strings containing info to be printed
    std::vector<String> runningProcessList; 
	std::vector<String> finishedProcessList;

	// Print all process names
	for (size_t i = 0; i < processScreenList.size(); i++)
	{
		std::shared_ptr<BaseScreen> screen = processScreenList[i];
		std::shared_ptr<Process> attachedProcess = screen->getProcess();
		String lsString;
		String cpuCore = std::to_string(attachedProcess->getCPUCoreID());
		String currentLine = std::to_string(attachedProcess->getCommandCounter());
		String totalLines = std::to_string(attachedProcess->getTotalCommandCounter());

		if (attachedProcess->isFinished())
		{
			lsString = attachedProcess->getName() + "\t(" + convertTime(attachedProcess->getTimeFinished()) + ")\t Core: " + cpuCore + "\t" + currentLine + " / " + totalLines;
			finishedProcessList.push_back(lsString);
		}
		else
		{
			// If attached process has no Time Started yet
			//if (attachedProcess->getTimeStarted() == 0)
				//lsString = attachedProcess->getName() + "\t" + "Not Yet Started" + "\t\t\t Core: -1" + "\t" + currentLine + " / " + totalLines;
			if (attachedProcess->getTimeStarted() != 0)
			{ // Process is running
				lsString = attachedProcess->getName() + "\t(" + convertTime(attachedProcess->getTimeStarted()) + ")\t Core: " + cpuCore + "\t" + currentLine + " / " + totalLines;
				runningProcessList.push_back(lsString);
			}
				
		}
		
		// Access the elements of BaseScreen using the screen pointer
		// Example: screen->onEnabled();
	}

	int utilizedCPUs = runningProcessList.size();
	int availableCPUS = getConfigNumCPU() - utilizedCPUs;// Total CPUs - utilizedCPUs
	int cpuUtil = utilizedCPUs * 100 / getConfigNumCPU();// Calculated by getting Cores Used * 100 / Total Cores

	std::ostringstream reportText; // Store everything printed into a single ostringstream

	reportText << "CPU Utilization: " << cpuUtil << "%" << std::endl;
	reportText << "Cores Utilized: " << utilizedCPUs << std::endl;
	reportText << "Cores Available: " << availableCPUS << std::endl << std::endl;

	reportText << "--------------------------------" << std::endl;
	// Print all running processes
	reportText << "Running Processes: " << std::endl;
	if (runningProcessList.size() == 0)
		reportText << "N/A" << std::endl;
	else
		for (size_t i = 0; i < runningProcessList.size(); i++)
		{
			reportText << runningProcessList[i] << std::endl;
		}

	// Print all finished processes
	reportText << "\nFinished Processes: " << std::endl;
	if (finishedProcessList.size() == 0)
		reportText << "N/A" << std::endl;
	else 
		for (size_t i = 0; i < finishedProcessList.size(); i++)
		{
			reportText << finishedProcessList[i] << std::endl;
		}
		
	
	reportText << "--------------------------------" << std::endl;

	// Print the report text
	if (printToConsole)
		std::cout << reportText.str() << std::endl;

	// Return for printing in report-util command
	return reportText.str();
}