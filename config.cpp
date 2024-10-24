#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

// Static (file-local) variables, only modifiable within this file
static int NUM_CPU = 4;
static std::string SCHEDULER = "rr";
static int QUANTUM_CYCLES = 5;
static int BATCH_PROCESS_FREQ = 1;
static int MIN_INS = 1000;
static int MAX_INS = 2000;
static int DELAY_PER_EXEC = 0;

// Initialization status is False on OS launch
static bool INIT_STATUS = false;

// Getter functions to access the values
int getConfigNumCPU() {
    return NUM_CPU;
}

std::string getConfigScheduler() {
    return SCHEDULER;
}

int getConfigQuantumCycles() {
    return QUANTUM_CYCLES;
}

int getConfigBatchProcessFreq() {
    return BATCH_PROCESS_FREQ;
}

int getConfigMinIns() {
    return MIN_INS;
}

int getConfigMaxIns() {
    return MAX_INS;
}

int getConfigDelayPerExec() {
    return DELAY_PER_EXEC;
}

bool isInitialized()
{
    return INIT_STATUS;
}

void setInitialized()
{
    INIT_STATUS = true;
}

void loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open config file, using default values." << std::endl;
        return;
    }

    std::unordered_map<std::string, std::string> configValues;
    std::string line;

    // Parse the config file
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string key;
        ss >> key;

        std::string value;
        ss >> value;
        configValues[key] = value;
    }

    // Assign values or use defaults if not found
    if (configValues.find("num-cpu") != configValues.end()) {
        NUM_CPU = std::stoi(configValues["num-cpu"]);
    }
    if (configValues.find("scheduler") != configValues.end()) {
        std::string scheduler = configValues["scheduler"];
        scheduler = scheduler.substr(1, scheduler.length() - 2); // Strips off the quotation marks
        SCHEDULER = scheduler;
    }
    if (configValues.find("quantum-cycles") != configValues.end()) {
        QUANTUM_CYCLES = std::stoi(configValues["quantum-cycles"]);
    }
    if (configValues.find("batch-process-freq") != configValues.end()) {
        BATCH_PROCESS_FREQ = std::stoi(configValues["batch-process-freq"]);
    }
    if (configValues.find("min-ins") != configValues.end()) {
        MIN_INS = std::stoi(configValues["min-ins"]);
    }
    if (configValues.find("max-ins") != configValues.end()) {
        MAX_INS = std::stoi(configValues["max-ins"]);
    }
    if (configValues.find("delay-per-exec") != configValues.end()) {
        DELAY_PER_EXEC = std::stoi(configValues["delay-per-exec"]);
    }
}
