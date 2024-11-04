#include "Process.h"
#include <fstream>
#include "PrintCommand.h"
#include <filesystem>
#include <iostream>
#include "Config.h"
#include <chrono>
#include <ctime>
#include <memory> 
#include <thread>


Process::Process(String name, int pid) : name(name), pid(pid) {}

String Process::getName() const
{
	return name;
}

int Process::getPid() const
{
	return pid;
}

int Process::getCommandCounter() const
{
	return this->commandCounter;
}

int Process::getRemainingTime() const // Remaining commands/time in the command list
{
	return this->commandList.size() - this->commandCounter;
}

int Process::getCPUCoreID() const
{
	return this->cpuCoreID;
}

Process::ProcessState Process::getState() const
{
	return this->currentState;
}

time_t Process::getTimeStarted() const
{
	return this->timeStarted;
}

time_t Process::getTimeFinished() const
{
	return this->timeFinished;
}

int Process::getTotalCommandCounter() const
{
	return this->commandList.size();
}

int Process::getDelayCounter() const
{
	return this->delayCounter;
}


bool Process::isFinished() const
{
	return this->commandCounter == this->commandList.size(); // If commandCounter is equal to the size of the commandList, the process is finished
}

void Process::addCommand(ICommand::CommandType commandType) // Add a command to the command list
{
	this->commandList.push_back(std::make_shared<ICommand>(pid, commandType));
}

void Process::setDelayCounter(int remainingDelay) // Set the delay counter
{
	//std::cout << "Setting delay counter to: " << remainingDelay << std::endl;
	this->delayCounter = remainingDelay;
}

void Process::setCPUCoreID(int coreID) {
	this->cpuCoreID = coreID;
}

void Process::setTimeStarted() {
    auto now = std::chrono::system_clock::now();
    this->timeStarted = std::chrono::system_clock::to_time_t(now);
}

void Process::setTimeFinished() {
    auto now = std::chrono::system_clock::now();
    this->timeFinished = std::chrono::system_clock::to_time_t(now);
}

void Process::setState(ProcessState state) {
	this->currentState = state;
}

void Process::executeCurrentCommand() // Remove const qualifier
{
	// Remove the const qualifier from this method to allow calling non-const member functions
	/*
	if (getDelayCounter() > 0) // If delay counter is greater than 0, decrement it and return
	{
		setDelayCounter(getDelayCounter()-1);
		//std::cout << "Setting delay counter to: " << getDelayCounter() << std::endl;
	}
	else
	*/
	// {
		// setDelayCounter(getConfigDelayPerExec()); // Reset delay counter
		//std::cout << "RESET DELAY COUNTER TO: " << this->delayCounter << std::endl;

		if (this->commandCounter >= this->commandList.size()) { // If the command counter is greater than the size of the command list, return
			std::cout << "Command List of process \"" + name + "\" is already finished!" << std::endl;
			return;
		};

		if (this->commandList[this->commandCounter]->getCommandType() == ICommand::PRINT)
		{
			//this->commandList[this->commandCounter]->execPrint();
			// Attempt to cast the ICommand pointer to a PrintCommand pointer
			PrintCommand* printCommand = dynamic_cast<PrintCommand*>(this->commandList[this->commandCounter].get());
			if (printCommand)
			{
				this->textBuffer += printCommand->execPrint(this->getCPUCoreID()); // Print and store in process' textBuffer
			}

			/* // Portion that prints out the process' text buffer to a file

			if ((this->commandCounter + 1) == this->commandList.size()) // If this command is the last command in the list, output the text buffer to a file
			{
				// Output text buffer as text file and clear it
				String filePath = this->name + "_output.txt";
				std::ofstream outFile(filePath);

				if (outFile.is_open())
				{
					outFile << this->textBuffer;
					outFile.close();
				}
				//std::cout << "Log File Output of \"" + name + "\" has been saved to: " << std::filesystem::absolute(filePath) << "!" << std::endl;

			}*/
		}
		else this->commandList[this->commandCounter]->execute();


		// int delay;
		// if (getConfigDelayPerExec() == 0) delay = 1*250;
		// else delay = getConfigDelayPerExec()*250;

		// std::this_thread::sleep_for(std::chrono::milliseconds(delay));



	// }
	
	
}

void Process::moveToNextLine() // Move to next command in the command list
{
	/*if (getDelayCounter() > 0) // If delay counter is greater than 0, decrement it and return
	{
		setDelayCounter(getDelayCounter() - 1);
		//std::cout << "Setting delay counter to: " << getDelayCounter() << std::endl;
	}*/
	//else
	//{
		//setDelayCounter(getConfigDelayPerExec()*2); // Reset delay counter
		this->commandCounter++;
	//}
}

void Process::populatePrintCommands(int limit) // Limit determines number of print commands to put in process
{
	for (int i = 0; i < limit; i++)
	{
		String toPrint = "Hello World from " + this->name+"!";
		const std::shared_ptr<ICommand> printCommand = std::make_shared<PrintCommand>(this->pid, toPrint);
		this->commandList.push_back(printCommand);
	}
}

// Round Robin
void Process::setRemainingQuantum(int quantum) {
	remainingQuantum = quantum;
}