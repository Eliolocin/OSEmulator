#include "ICommand.h"

ICommand::ICommand(int pid, CommandType type) : pid(pid), commandType(type) {}

ICommand::CommandType ICommand::getCommandType() const
{ 
	return this->commandType;
}

void ICommand::execute()
{
	// Do nothing
}