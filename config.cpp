#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

// Static (file-local) variables, only modifiable within this file
// Default values are set here
static int NUM_CPU = 4;
static std::string SCHEDULER = "rr";
static int QUANTUM_CYCLES = 5;
static int BATCH_PROCESS_FREQ = 1;
static int MIN_INS = 1000;
static int MAX_INS = 2000;
static int DELAY_PER_EXEC = 0;
static size_t MAX_OVERALL_MEMORY = 16384;
static size_t MEM_PER_FRAME = 16;
static size_t MIN_MEM_PER_PROCESS = 1024;
static size_t MAX_MEM_PER_PROCESS = 4096;

// How many actual 'ticks' it takes for one CPU cycle in the OS to pass
// Made to help prevent bugs from CPU running to fast
const int CYCLE_SPEED = 5000; 

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
    return QUANTUM_CYCLES*CYCLE_SPEED;
}

int getConfigBatchProcessFreq() {
    return (BATCH_PROCESS_FREQ+1) * CYCLE_SPEED;
}

int getConfigMinIns() {
    return MIN_INS;
}

int getConfigMaxIns() {
    return MAX_INS;
}

int getConfigDelayPerExec() {
    return DELAY_PER_EXEC * CYCLE_SPEED;
}

size_t getConfigMaxOverallMemory() {
	return MAX_OVERALL_MEMORY;
}

size_t getConfigMemPerFrame() {
	return MEM_PER_FRAME;
}

size_t getConfigMinMemPerProcess() {
	return MIN_MEM_PER_PROCESS;
}

size_t getConfigMaxMemPerProcess() {
	return MAX_MEM_PER_PROCESS;
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
        if (std::stoi(configValues["delay-per-exec"]) == 0) {
            DELAY_PER_EXEC = 1;
        } else {
            DELAY_PER_EXEC = std::stoi(configValues["delay-per-exec"]);
        }
    }
	if (configValues.find("max-overall-mem") != configValues.end()) {
		MAX_OVERALL_MEMORY = std::stoi(configValues["max-overall-mem"]);
	}
	if (configValues.find("mem-per-frame") != configValues.end()) {
		MEM_PER_FRAME = std::stoi(configValues["mem-per-frame"]);
	}
    if (configValues.find("min-mem-per-proc") != configValues.end()) {
        MIN_MEM_PER_PROCESS = std::stoi(configValues["min-mem-per-proc"]);
	}
	if (configValues.find("max-mem-per-proc") != configValues.end()) {
		MAX_MEM_PER_PROCESS = std::stoi(configValues["max-mem-per-proc"]);
	}
	file.close();
}
