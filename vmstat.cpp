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
#include "GlobalScheduler.h"


void VMStat()
{
    // Get all processes through their console
    std::vector<std::shared_ptr<BaseScreen>> processScreenList = ConsoleManager::getInstance()->getAllProcessScreens();

    // Gather memory information from all processes
    size_t totalMemory = getConfigMaxOverallMemory();
    size_t usedMemory = 0;
    struct ProcessMemoryInfo {
        std::string processName;
        size_t memoryUsed;
    };
    std::vector<ProcessMemoryInfo> processMemoryList; // Stores memory usage for each process

    for (size_t i = 0; i < processScreenList.size(); i++) {
        std::shared_ptr<BaseScreen> screen = processScreenList[i];
        std::shared_ptr<Process> process = screen->getProcess();
        if (process && process->isMemoryAllocated()) {
            size_t memoryUsed = process->getMemoryRequired();
            usedMemory += memoryUsed;
            processMemoryList.push_back({ process->getName(), memoryUsed });
        }
    }

    // CPU Utilization percentage

    // Create the output buffer
    std::ostringstream textBuffer;
    textBuffer << "===== ===== ===== ===== =====\n";
    textBuffer << "Total Memory: " << totalMemory << " KB\n";
    textBuffer << "Used Memory: " << usedMemory << " KB\n";
    textBuffer << "Free Memory: " << totalMemory-usedMemory << " KB\n";
    textBuffer << "Idle CPU Ticks: " << getIdleTicks() << "\n";
    textBuffer << "Active CPU Ticks: " << getActiveTicks() << "\n";
    textBuffer << "Total CPU Ticks: " << getActiveTicks()+getIdleTicks() << "\n";
    textBuffer << "Pages Paged In: " << getInPages() << "\n";
    textBuffer << "Pages Paged Out: " << getOutPages() << "\n";
    textBuffer << "===== ===== ===== ===== =====\n";


    std::cout << textBuffer.str() << std::endl;

}