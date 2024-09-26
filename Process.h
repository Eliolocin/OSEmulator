#pragma once
#include "TypeDefs.h"

class Process // Class representing a Process in the OS, identified with an ID and a name
{
public:
	Process(String name, int pid, int currentLine, int totalLines, time_t timeStarted, time_t timeFinished); // Constructor of Process
	String getName() const; // Getter for name
	int getPid() const; // Get the process ID
	int getCurrentLine() const; // Get the current line of the process
	int getTotalLines() const; // Get the total lines of the process
	time_t getTimeStarted() const; // Get the time the process started
	time_t getTimeFinished() const; // Get the time the process finished
	


private:
	String name; // Name of the process
	int pid; // Process ID
	int currentLine; // Current line of the process
	int totalLines; // Total lines of the process
	time_t timeStarted; // Time the process started
	time_t timeFinished; // Time the process finished
};

