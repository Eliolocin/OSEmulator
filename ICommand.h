#pragma once

class ICommand // Interface for a command
{
public:
	enum CommandType // Enum representing the type of command
	{
		IO,
		PRINT
	};

	ICommand(int pid, CommandType type); // Constructor of ICommand
	CommandType getCommandType() const;
	virtual void execute(); // Execute the command

protected:
	int pid; // Process ID
	CommandType commandType; // Type of command
};