#pragma once
#include "AbsScheduler.h"
#include "SchedulerWorker.h"
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <chrono>
#include <atomic>
#include <map>
#include "Config.h"

enum SchedulerType {
    FCFS,
    ROUND_ROBIN
};

class GlobalScheduler : public AbsScheduler {
public:
    GlobalScheduler(int numWorkers);  // Initialize scheduler with number of cores
    void scheduleProcess(std::shared_ptr<Process> process) override;  // FCFS algorithm implementation
    void start() override;  // Start the scheduler
    void stop() override;  // Stop the scheduler
    
    int getQueueSize(); // Add function to get queue size for debugging
    void startTestMode(); // Starts generating dummy processes
    void stopTestMode(); // Stops generating dummy processes
    void notifyWorkerFree();

    int getDelayCounter(); // Add function to get delay counter for debugging
    void setDelayCounter(int remainingDelay); // Set the delay counter
    
private:
    std::queue<std::shared_ptr<Process>> processQueue;  // Queue to hold processes
    std::vector<std::shared_ptr<SchedulerWorker>> workers;  // List of CPU workers
    void dispatchProcesses();  // Internal function to dispatch processes to available workers
    std::mutex queueMutex;  // Mutex for protecting access to the process queue
    std::thread schedulerThread;  // Thread for the scheduler
    
    std::condition_variable processAvailable;  // Notifies when a new process is added
    std::atomic<bool> stopRequested{ false };  // Flag to signal the scheduler thread to stop
    std::atomic<bool> testModeActive{ false }; // Flag to control scheduler-test
    std::thread testThread; // Thread for scheduler-test to generate processes

    //void dispatchFCFS(); // FCFS scheduling method
    void dispatchRoundRobin(); // Round Robin scheduling method
    SchedulerType schedulerType; // New variable to store scheduler type
    int quantumCycles; // Quantum cycles for Round Robin

	int delayCounter = getConfigBatchProcessFreq(); // Delay counter to simulate delay between commands
    
    //std::vector<std::atomic<bool>> cpuCores;
    //std::queue<std::shared_ptr<Process>> waitingQueue; // Queue for Round Robin processes
    //bool stopRequested = false;                    // To signal the scheduler to stop
    //std::map<int, std::thread> runningProcesses;  // Maps process IDs to threads
};
