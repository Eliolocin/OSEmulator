#pragma once
#include "GlobalScheduler.h"
// File that contains utilities for the project

void SetTextColor(std::string color);
void WelcomeUser(std::string osname);
std::string convertTime(time_t time);
void generateHundredPrints(int numOfProcesses, GlobalScheduler& scheduler);

int randomNumber(int min, int max);
size_t randomMemSize(size_t min, size_t max);