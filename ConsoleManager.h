#pragma once

#include "AbsConsole.h"
#include "BaseScreen.h"
#include "TypeDefs.h"
#include "GlobalScheduler.h"

const String MAIN_CONSOLE = "MAIN_CONSOLE";
const String MARQUEE_CONSOLE = "MARQUEE_CONSOLE";
const String SCHEDULING_CONSOLE = "SCHEDULING_CONSOLE";
const String MEMORY_CONSOLE = "MEMORY_CONSOLE";

class ConsoleManager // Singleton Class that stores, manages, and controls all Console screens
{
public:
	// ConsoleTable is a hashmap that stores all possible console screens such as Main Menu, Marquee, etc. after they are instantiated
	// The key is a String that represents the name of the console screen
	// The value is a shared pointer to an AbsConsole object
	typedef std::unordered_map<String, std::shared_ptr<AbsConsole>> ConsoleTable;

	static ConsoleManager* getInstance(); // Singleton class
	static void initialize(); // Instantiate Singleton class
	static void destroy(); // Destroy Singleton class

	void drawConsole() const;

	void switchConsole(String consoleName);

	void process() const;

	bool isRunning() const;
	void setRunning(bool running);

	int getConsoleTableSize() const;

	void registerScreen(std::shared_ptr<BaseScreen> console, bool switchToScreen = true); // Register a console screen to the ConsoleTable
	void switchToScreen(String screenName); // Switch to a console screen
	void unregisterScreen(String screenName); // Unregister a console screen from the ConsoleTable
	std::vector<std::shared_ptr<BaseScreen>> getAllProcessScreens() const; // Returns all base screens
		
	//void setGlobalScheduler(std::shared_ptr<GlobalScheduler> globalScheduler);
	void setGlobalScheduler(GlobalScheduler* scheduler); // Set GlobalScheduler using a raw pointer
	GlobalScheduler* getGlobalScheduler() const; // Get GlobalScheduler using a raw pointer

private:
	ConsoleManager(); // Default constructor
	~ConsoleManager() = default; // Default destructor
	ConsoleManager(ConsoleManager const&) {}; // Copy constructor
	ConsoleManager& operator=(ConsoleManager const&) {}; // Assignment operator
	static ConsoleManager* sharedInstance; // Singleton instance of the class

	ConsoleTable consoleTable;
	std::shared_ptr<AbsConsole> currentConsole;
	std::shared_ptr<AbsConsole> previousConsole;

	//std::shared_ptr<GlobalScheduler> globalScheduler;

	bool running = true;
	GlobalScheduler* globalScheduler = nullptr; // Use raw pointer
};