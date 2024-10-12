#pragma once
#include "AbsScheduler.h"
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <thread>
#include <memory>


class SchedulerWorker {
public:
    SchedulerWorker(int id);  // Constructor takes the CPU Core ID
    void assignProcess(std::shared_ptr<Process> process);  // Assign a process to this worker
    void start();  // Start the worker thread
    void stop();  // Stop the worker
    bool isBusy() const;  // Check if worker is busy
    void processExecution();  // Main function to execute process
    void notify();  // Add this method to wake up the worker

	int getWorkerId();  // Get the worker ID (Core ID)



private:
    std::shared_ptr<Process> assignedProcess;
    std::atomic<bool> running;
    std::atomic<bool> busy;
    std::thread workerThread;
    int workerId;  // Core ID for this worker
    std::mutex mtx;
    std::condition_variable cv;
};
