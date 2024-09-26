#include "ConsoleManager.h"
#include <iostream>
#include "TypeDefs.h"
#include "Process.h"
#include "AbsConsole.h"
#include "MainConsole.h"
#include "Utilities.h"

ConsoleManager* ConsoleManager::sharedInstance = nullptr;
ConsoleManager* ConsoleManager::getInstance()
{
	return sharedInstance;
}

ConsoleManager::ConsoleManager()
{
	this->running = true;
	//Initialize Consoles

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();

	this->consoleTable[MAIN_CONSOLE] = mainConsole;

	this->switchConsole(MAIN_CONSOLE);
}

void ConsoleManager::initialize()
{
	if (sharedInstance == nullptr)
	{
		sharedInstance = new ConsoleManager();
	}
}

void ConsoleManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void ConsoleManager::drawConsole() const
{
	if (this->currentConsole != nullptr)
	{
		this->currentConsole->display(); // Keeps running the display function of the current console
	}
	else
	{
		std::cout << "No console screen is currently active." << std::endl;
	}
}

void ConsoleManager::switchConsole(String consoleName)
{
	if (this->consoleTable.contains(consoleName))
	{
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[consoleName];
		this->currentConsole->onEnabled();
	}
	else
	{
		std::cout << "Screen with name " << consoleName << " does not exist." << std::endl;
	}
}

bool ConsoleManager::isRunning() const
{
	return this->running;
}

void ConsoleManager::setRunning(bool running)
{
	this->running = running;
}

void ConsoleManager::process() const
{
	if (this->currentConsole != nullptr)
	{
		this->currentConsole->process(); // Keeps running the current console's process
	}
	else
	{
		std::cout << "No console screen is currently active." << std::endl;
	}
}

void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> screenRef)
{
	if (this->consoleTable.contains(screenRef->getName()))
	{
		std::cout << "Screen with name " << screenRef->getName() << " already exists." << std::endl;
	}
	else
	{
		this->consoleTable[screenRef->getName()] = screenRef;
	}
}

void ConsoleManager::unregisterScreen(String screenName)
{
	if (this->consoleTable.contains(screenName))
	{
		this->consoleTable.erase(screenName);
	}
	else
	{
		std::cout << "Screen with name " << screenName << " does not exist." << std::endl;
	}
}
