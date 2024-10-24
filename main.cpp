#include <iostream>

#include "ConsoleManager.h"
#include "GlobalScheduler.h"
#include "Utilities.h"
#include "Config.h"

int main() // Main entry point of OS
{
    std::string OSName = "CSOPESY";
    std::string configFile = "config.txt";
    std::string Command;
    bool running = true; // Program keeps running while True

    ConsoleManager::initialize(); // Initialize ConsoleManager Singleton
    loadConfig(configFile); // Load configuration file
	GlobalScheduler scheduler(getConfigNumCPU()); // Create GlobalScheduler with number of CPU cores


    // Test cases
    //generateHundredPrints(10, scheduler); // Generate 10 processes with 100 print commands each

    scheduler.start(); // Start the scheduler using the shared pointer

    while (running)
    {
        ConsoleManager::getInstance()->process();
        ConsoleManager::getInstance()->drawConsole();

        running = ConsoleManager::getInstance()->isRunning();
    }

    scheduler.stop(); // Stop the scheduler using the shared pointer

    return 0;
}
