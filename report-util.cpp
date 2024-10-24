#include <filesystem>
#include <fstream>
#include <iostream>
#include "Commands.h"	
#include "Screen.h"
#include "Utilities.h"

void ReportUtil()
{
	String filePath = "csopesy-log.txt";
	std::ofstream outFile(filePath);
	if (outFile.is_open())
	{
		outFile << ScreenLS(false);
		outFile.close();
	}
	std::cout << "Report generated at " << std::filesystem::absolute(filePath) << "!" << std::endl;
}