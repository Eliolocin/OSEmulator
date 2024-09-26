#include "Process.h"

Process::Process(String name, int pid, int currentLine, int totalLines, time_t timeStarted, time_t timeFinished) : name(name), pid(pid), currentLine(currentLine), totalLines(totalLines), timeStarted(timeStarted), timeFinished(timeFinished) {}

String Process::getName() const { return name; }

int Process::getPid() const { return pid; }

int Process::getCurrentLine() const { return currentLine; }

int Process::getTotalLines() const { return totalLines; }

time_t Process::getTimeStarted() const { return timeStarted; }

time_t Process::getTimeFinished() const { return timeFinished; }

