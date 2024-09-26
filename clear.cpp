
#include <iostream>
#include "Commands.h"	
#include "Utilities.h"

void Clear() 
{
	//For the clear command, once recognized, the screen clears, reprinting the header texts

	#ifdef _WIN32
		system("CLS");  // Windows-specific command to clear the console
	#else
		system("clear");  // Unix-based systems' command to clear the console
	#endif

	// Reprint the header and welcome message
	WelcomeUser("CSOPESY");
}