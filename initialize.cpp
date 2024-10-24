
#include <iostream>
#include "Commands.h"	
#include "Utilities.h"
#include "Config.h"
#include "ConsoleManager.h"
#include "GlobalScheduler.h"


void Initialize()
{
	if (isInitialized())
		std::cout << "OS is initialized already!\n";
	else
	{ // Initialize system
		setInitialized();
		std::cout << "OS Initialization Successful!\n";
	}
		
}