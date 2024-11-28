#pragma once
#include "TypeDefs.h"
#include "ICommand.h"
#include "Config.h"

class Process // Class representing a Process in the OS, identified with an ID and a name
{
public:
	struct RequirementFlags
	{
		bool requireFiles;
		bool requireMemory;
	};

	enum ProcessState // Enum representing the state of the process
	{
		READY,
		RUNNING,
		WAITING,
		FINISHED
	};

	Process(String name, int pid, size_t memoryRequired, size_t frameSize); // Constructor of Process
	void addCommand(ICommand::CommandType commandType); // Add a command to the process
	void executeCurrentCommand(); // Execute the current command of the process
	void moveToNextLine(); // Move to next line of command
	void populatePrintCommands(int limit); // Fill process with Print test commands

	bool isFinished() const; // Check if the process is finished
	int getRemainingTime() const; // Get the remaining time of the process
	int getCommandCounter() const; // Get the command counter (Current Line of the Process)
	int getTotalCommandCounter() const; // Get the total commands in Process
	int getCPUCoreID() const; // Get the CPU core ID
	ProcessState getState() const; // Get the state of the process
	String getName() const; // Getter for name
	int getPid() const; // Get the process ID

	//int getCurrentLine() const; // Get the current line of the process
	//int getTotalLines() const; // Get the total lines of the process
	time_t getTimeStarted() const; // Get the time the process started
	time_t getTimeFinished() const; // Get the time the process finished

	int getDelayCounter() const; // Get the delay counter
	void setDelayCounter(int remainingDelay); // Set the delay counter

	void setCPUCoreID(int coreID); // Set the CPU core ID
	void setTimeStarted();  // Set the start time
	void setTimeFinished();  // Set the finish time
	void setState(ProcessState state);  // Set the process state

	// Memory related methods
	size_t getMemoryRequired() const; // Get the memory required for the process
	void setMemoryRequired(size_t memory); // Set the memory required for the process

	// Round Robin related methods
	int getRemainingQuantum() const { return remainingQuantum; } // Get the remaining quantum for Round Robin
	void setRemainingQuantum(int quantum); // Set the remaining quantum for Round Robin
	void decrementQuantum() { remainingQuantum--; } // Decrement the remaining quantum by 1
	void resetQuantum(int quantumCycles) { remainingQuantum = quantumCycles; }

	//memory
	bool isMemoryAllocated() const { return memoryAllocated; }
	void setMemoryAllocated(bool allocated) { memoryAllocated = allocated; }

	void* getAllocatedMemory() const { return allocatedMemory; }
	void setAllocatedMemory(void* memory) { allocatedMemory = memory; }

	size_t getNumFramesNeeded() const;  // Getter for numFramesNeeded

private:
	String name; // Name of the process
	int pid; // Process ID
	size_t memoryRequired; // Memory required to run the process

	int delayCounter = getConfigDelayPerExec(); // Delay counter to simulate delay between commands

	typedef std::vector<std::shared_ptr<ICommand>> CommandList; // List of commands the process will execute
	CommandList commandList;

	int commandCounter = 0;
	// Current line of code the process is executing (cumulative of all commands)
	// Is also equivalent to the number of processes/commands the process has executed

	int cpuCoreID = 0; // ID of the CPU core the process is running on

	mutable String textBuffer = "Process name: " + name + "\nLogs:\n\n"; // Buffer to store the logs of the process
	// Mutable to bypass const restraint of functions that are not supposed to modify the object

	RequirementFlags requirementFlags = { false, false };  // Default values
	ProcessState currentState = ProcessState::READY;  // Start in READY state


	//int currentLine; // Current line of the process
	//int totalLines; // Total lines of the process
	time_t timeStarted = 0; // Time the process started NOT when it was created (to be adjusted by Scheduler)
	time_t timeFinished = 0; // Time the process finished (to be adjusted by Scheduler)

	int remainingQuantum = 0; // Quantum time left for Round Robin scheduling

	//memory
	bool memoryAllocated = false;  // Track if memory has already been allocated
	void* allocatedMemory = nullptr;  // Pointer to the allocated memory
	size_t numFramesNeeded;  // Frames needed for this process
};

