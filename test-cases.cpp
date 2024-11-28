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
#include "GlobalScheduler.h"

void generateHundredPrints(int numOfProcesses, GlobalScheduler& scheduler) {
    int minInstructions = getConfigMinIns();
    int maxInstructions = getConfigMaxIns();
    size_t minMemPerProcess = getConfigMinMemPerProcess();
    size_t maxMemPerProcess = getConfigMaxMemPerProcess();

    for (int i = 0; i < numOfProcesses; i++) {
        String processName = "process_" + std::to_string(i);
        size_t processMemReq = randomMemSize(minMemPerProcess, maxMemPerProcess);
        size_t frameSize = getConfigMemPerFrame();
        auto newProcess = std::make_shared<Process>(processName, i, processMemReq, frameSize);  // Create new process

    	newProcess->populatePrintCommands(100);  // Add dummy Print commands

        BaseScreen newScreen(newProcess, processName);
        ConsoleManager::getInstance()->registerScreen(std::make_shared<BaseScreen>(newScreen));

        // Add the process to the scheduler's queue
        //std::cout << processName << "\" ADDED FIRST" << std::endl;
        scheduler.scheduleProcess(newProcess);

        std::cout << "Process \"" << processName << "\" generated w/ 100 print commands and added to the scheduler queue." << std::endl;
    }
    std::cout << "\n";
}

