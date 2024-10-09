#include "PrintCommand.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include "Utilities.h"

PrintCommand::PrintCommand(int pid, String& toPrint) : ICommand(pid, PRINT)
{
    this->toPrint = toPrint;
}

String PrintCommand::execPrint(int cpuId)
{
    ICommand::execute();

	// std::cout << "PID " << this->pid << ":" << this->toPrint << std::endl;
    auto now = std::chrono::system_clock::now();
    time_t execTime = std::chrono::system_clock::to_time_t(now);
    String formattedTime = "("+convertTime(execTime)+")";
    String msg = formattedTime + " Core:" + std::to_string(cpuId) + " \"" + this->toPrint + "\"\n";
    //String msg = formattedTime + " Core:" + std::to_string(cpuId) + " PID " + std::to_string(this->pid) + ": " + this->toPrint + "\n";
    //msg << this->toPrint << "\n";
	//std::cout << msg;

	return msg;
}