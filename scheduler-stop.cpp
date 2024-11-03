
#include <iostream>
#include "Commands.h"	
#include "Utilities.h"
#include "ConsoleManager.h"

void SchedulerStop()
{
	//std::cout << "Scheduler-stop command recognized. Doing something...\n";
    GlobalScheduler* scheduler = ConsoleManager::getInstance()->getGlobalScheduler();
    if (scheduler) {
        scheduler->stopTestMode();
        std::cout << "Scheduler-test mode deactivated. Stopped generating dummy processes." << std::endl;
    }
    else {
        std::cerr << "Scheduler is not available!" << std::endl;
    }
}