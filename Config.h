#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <string>

int getConfigNumCPU();
std::string getConfigScheduler();
int getConfigQuantumCycles();
int getConfigBatchProcessFreq();
int getConfigMinIns();
int getConfigMaxIns();
int getConfigDelayPerExec();

bool isInitialized();
void setInitialized();

void loadConfig(const std::string& filename);

#endif

