#include "GlobalScheduler.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <queue>

// Constructor initializes the number of workers (cores)
GlobalScheduler::GlobalScheduler(int numWorkers) {
    for (int i = 0; i < numWorkers; ++i) {
        workers.push_back(std::make_shared<SchedulerWorker>(i));  // Create workers (CPU cores)
    }
}

// Add a process to the queue for scheduling
void GlobalScheduler::scheduleProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(queueMutex);
    processQueue.push(process);  // Add the process to the queue
}

// Start the scheduler in thread
void GlobalScheduler::start() {
    // Start all worker threads
    for (auto& worker : workers) {
        worker->start();  // Start each worker in each thread
    }

    // Start the scheduler in a separate thread to manage process dispatching
    schedulerThread = std::thread(&GlobalScheduler::dispatchProcesses, this);
}

// Scheduler logic for dispatching processes to workers
void GlobalScheduler::dispatchProcesses() {
    while (!processQueue.empty()) {
        for (auto& worker : workers) {
            if (!worker->isBusy() && !processQueue.empty()) {
                // Lock the queue to prevent race conditions
                std::lock_guard<std::mutex> lock(queueMutex);

                // Get the first process in the queue (FCFS)
                auto process = processQueue.front();
                processQueue.pop();  // Remove it from the queue

                // Debug
                // std::cout << "Assigning process " << process->getName() << " to worker " << worker->getWorkerID() << std::endl;
                // Assign the process to the worker and notify the worker to start
                worker->assignProcess(process);
                worker->notify();  // Notify the worker thread to start processing
            }
        }
        // Sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Stop the scheduler and all worker threads
void GlobalScheduler::stop() {
    for (auto& worker : workers) {
        worker->stop();
    }

    if (schedulerThread.joinable()) {
        schedulerThread.join();  // Ensure the scheduler thread is properly joined
    }
}
