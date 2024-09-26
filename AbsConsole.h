#pragma once
#include "TypeDefs.h"

class AbsConsole // Abstract class of Console screens
{
public:
	AbsConsole(String name); // Constructor
	~AbsConsole() = default; // Destructor

	String getName(); // Getter for name
	virtual void onEnabled() = 0; // Called when the console is enabled
	virtual void display() = 0; // Called per frame
	virtual void process() = 0; // Process function

	String name; // Name of the console screen
	friend class ConsoleManager; // Allows AbsConsole to access private members of ConsoleManager, vice-versa
	
};

