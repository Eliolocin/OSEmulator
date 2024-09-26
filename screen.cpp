
#include <iostream>
#include "Commands.h"	
#include "Utilities.h"
#include "Screen.h"
#include "TypeDefs.h"
#include "Process.h"
#include "ConsoleManager.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <ctime>

void ScreenR(String processName)
{
	ConsoleManager::getInstance()->switchConsole(processName);
}

void ScreenS(String processName)
{
	auto now = std::chrono::system_clock::now();

	// Convert the time_point to a time_t, which represents the time in seconds since the epoch
	time_t startTime = std::chrono::system_clock::to_time_t(now);
	time_t endTime = 0;
	// Convert the time_t to a tm structure
	//std::tm* startTime = std::localtime(&currentTime);
	//std::tm* endTime = 0;

	Process newProcess = Process(processName, rand() % 51, 0, rand() % 1001, startTime, endTime);
	BaseScreen newScreen = BaseScreen(std::make_shared<Process>(newProcess), processName);

	ConsoleManager::getInstance()->registerScreen(std::make_shared<BaseScreen>(newScreen));
	ConsoleManager::getInstance()->switchConsole(processName);
}