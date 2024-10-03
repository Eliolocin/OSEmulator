#include <iostream>
#include <thread>
#include <conio.h>
#include <windows.h>
#include <string>
#include <vector>

class MarqueeConsole {
public:
    void Run();
    std::string ProcessCommand(const std::string& command);
    void RunNonThreaded();
private:
    void PollKeyboard();
    void RedrawScreen();
    std::vector<std::string> commandHistory; // Store previous commands and outputs
    int currentLine = 13; // Keeps track of the current line for new input
};

class MarqueeWorkerThread {
public:
    void PrintHeader();
    void StartMarquee();
};

class ConsoleManager {
public:
    void CheckBounds(int& x, int& y, int& dx, int& dy);
    void SetCursorPosition(int x, int y);
};

void ConsoleManager::CheckBounds(int& x, int& y, int& dx, int& dy) {
    if (x <= 0 || x >= 50) dx = -dx;
    if (y <= 3 || y >= 10) dy = -dy;
}

void ConsoleManager::SetCursorPosition(int x, int y) {
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void MarqueeWorkerThread::PrintHeader() {
    ConsoleManager consoleManager;

    // Set header position at the top (row 0)
    consoleManager.SetCursorPosition(0, 0);
    std::cout << "*********************************" << std::endl;

    consoleManager.SetCursorPosition(0, 1);
    std::cout << "* Displaying a marquee console! *" << std::endl;

    consoleManager.SetCursorPosition(0, 2);
    std::cout << "*********************************" << std::endl;
}

void MarqueeWorkerThread::StartMarquee() {
    int x = 25, y = 5, dx = 1, dy = 1;
    ConsoleManager manager;

    while (true) {
        // Clear the screen before every update
        system("cls");

        // 1. Print header at the top of the screen
        PrintHeader();

        // 2. Increment cursor position of the marquee text
        manager.CheckBounds(x, y, dx, dy);

        // 3. Set the cursor to the (x, y) for the bouncing text
        manager.SetCursorPosition(x, y);
        std::cout << "Hello world in marquee!";

        // 4. Update position of marquee text
        x += dx;
        y += dy;

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // adjust refresh rate
    }
}

void MarqueeConsole::RedrawScreen() {
    ConsoleManager consoleManager;

    // Clear screen
    system("cls");

    // Redraw the header
    MarqueeWorkerThread worker;
    worker.PrintHeader();

    // Redraw command history
    for (int i = 0; i < commandHistory.size(); ++i) {
        consoleManager.SetCursorPosition(0, 12 + i);
        std::cout << commandHistory[i];
    }

    // Set the new line for input after the last command output
    currentLine = 12 + commandHistory.size();
}

void MarqueeConsole::PollKeyboard() {
    std::string commandBuffer;
    char ch;
    ConsoleManager consoleManager;

    while (true) {
        RedrawScreen();  // redraw the entire screen including the command history

        // 5. Set position of the command input field at the bottom of the current section
        consoleManager.SetCursorPosition(0, currentLine);
        std::cout << "Enter a Command for Marquee_Console: " << commandBuffer << " ";

        // Poll for keyboard input
        if (_kbhit()) {
            ch = _getch();

            // Check if Enter is pressed
            if (ch == '\r') {
                std::string fullCommand = "Enter a Command for Marquee_Console: " + commandBuffer;
                commandHistory.push_back(fullCommand);  // Save the entered command

                std::string result = ProcessCommand(commandBuffer);  // Process command
                commandHistory.push_back(result);  // Save the result/output of the command
                if (result == "Exiting program...") {
                    exit(0);
                }

                commandBuffer.clear();  // Clear the command buffer after processing
            }
            // Handle backspace input
            else if (ch == '\b' && !commandBuffer.empty()) {
                commandBuffer.pop_back();
            }
            // Append typed characters to the command buffer
            else if (ch != '\b') {
                commandBuffer += ch;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Adjust for smoother input polling
    }
}

std::string MarqueeConsole::ProcessCommand(const std::string& command) {
    // Example commands
    if (command == "exit") {
        return "Exiting program...";
    }
    else if (command == "help") {
        return "Command processed in Marquee_Consoles: Recognized commands are- exit, help";
    }
    else {
        return "Command processed in Marquee_Console: This is a sample barebones immediate mode UI drawing.";
    }
}

void MarqueeConsole::Run() {
    // Start the marquee in a separate thread
    std::thread marqueeThread(&MarqueeWorkerThread::StartMarquee, MarqueeWorkerThread());
    marqueeThread.detach(); // Allows animation and input to run in parallel

    // Poll for keyboard events in the main thread
    PollKeyboard();
}

void MarqueeConsole::RunNonThreaded() {
    MarqueeWorkerThread worker;
    while (true) {
        // Poll for keyboard input 
        PollKeyboard();  // blocks the marquee from updating while it handles input

        // After handling input, update the marquee 
        worker.StartMarquee();  // Sequential execution: the marquee updates only after input is handled
    }
}

int main() {
    MarqueeConsole console;
    console.Run();
    return 0;
}
