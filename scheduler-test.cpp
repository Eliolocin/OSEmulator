
#include <iostream>
#include "Commands.h"	
#include "Utilities.h"
#include "ConsoleManager.h"

void SchedulerTest()
{
	//std::cout << "Scheduler-test command recognized. Doing something...\n";
    GlobalScheduler* scheduler = ConsoleManager::getInstance()->getGlobalScheduler();
    if (scheduler) {
        scheduler->startTestMode();
        std::cout << "Scheduler-test mode activated. Generating dummy processes..." << std::endl;
    }
    else {
        std::cerr << "Scheduler is not available!" << std::endl;
    }
}