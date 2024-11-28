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
size_t getConfigMaxOverallMemory();
size_t getConfigMemPerFrame();
size_t getConfigMinMemPerProcess();
size_t getConfigMaxMemPerProcess();

int getActiveTicks();
int getIdleTicks();
int getInPages();
int getOutPages();

void incActiveTicks();
void incIdleTicks(int increase);
void decIdleTicks();
void incInPages();
void incOutPages();

bool isInitialized();
void setInitialized();

void loadConfig(const std::string& filename);

#endif

