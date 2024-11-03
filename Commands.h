#pragma once

#include "TypeDefs.h"
#include "GlobalScheduler.h"
// File that contains commands for the project

void Clear();
void Exit();
void Initialize();
void ReportUtil();
void SchedulerStop();
void SchedulerTest();
void ScreenR(std::string processName);
void ScreenS(std::string processName, GlobalScheduler* scheduler);
std::string ScreenLS(bool printToConsole);
void ProcessSmi();