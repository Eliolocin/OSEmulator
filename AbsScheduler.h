#pragma once
#include "TypeDefs.h"
#include <thread>
#include <vector>
#include <memory>
#include "Process.h"

class AbsScheduler {
public:
    virtual void scheduleProcess(std::shared_ptr<Process> process) = 0; // Abstract function for scheduling a process
    virtual void start() = 0;  // Start the scheduler
    virtual void stop() = 0;  // Stop the scheduler
    virtual ~AbsScheduler() = default;
};
