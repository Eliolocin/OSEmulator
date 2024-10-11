#pragma once
#include "AbsScheduler.h"
#include "SchedulerWorker.h"
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <chrono>


class GlobalScheduler : public AbsScheduler {
public:
    GlobalScheduler(int numWorkers);  // Initialize scheduler with number of cores
    void scheduleProcess(std::shared_ptr<Process> process) override;  // FCFS algorithm implementation
    void start() override;  // Start the scheduler
    void stop() override;  // Stop the scheduler

private:
    std::queue<std::shared_ptr<Process>> processQueue;  // Queue to hold processes
    std::vector<std::shared_ptr<SchedulerWorker>> workers;  // List of CPU workers
    void dispatchProcesses();  // Internal function to dispatch processes to available workers
    std::mutex queueMutex;  // Mutex for protecting access to the process queue
    std::thread schedulerThread;  // Thread for the scheduler
};
