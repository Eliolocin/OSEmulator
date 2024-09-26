#pragma once
#include "TypeDefs.h"
#include "AbsConsole.h"
#include "Process.h"


class BaseScreen: public AbsConsole // Actual implementation of the console screens
{
public:
	BaseScreen(std::shared_ptr<Process> process, String processName); // Constructor of Screen that has a process and its process name
	void onEnabled() override;	// Called when the console is enabled
	void display() override; // Called per frame
	void process() override; // Process function

private:
	void printProcessInfo() const; // Print process information
	std::shared_ptr<Process> attachedProcess; // Process attached to the screen
	bool refreshed = false; // Flag to check if the screen has been refreshed
};

