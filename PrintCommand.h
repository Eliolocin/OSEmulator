#pragma once
#include "ICommand.h"
#include "TypeDefs.h"

class PrintCommand : public ICommand
{
public:
	PrintCommand(int pid, String& toPrint); // Constructor of PrintCommand
	String execPrint(int cpuId); // Execute the command
private:
	String toPrint;
};