#include "GlobalScheduler.h"
#include "Config.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <queue>
#include "ConsoleManager.h"

// Constructor initializes the number of workers (cores)
GlobalScheduler::GlobalScheduler(int numWorkers) 
    : schedulerType(getConfigScheduler() == "rr" ? ROUND_ROBIN : FCFS),
    quantumCycles(getConfigQuantumCycles()) {
    for (int i = 0; i < numWorkers; ++i) {
        //std::cout << "CPU \"" << i << "\" added!" << std::endl;
        workers.push_back(std::make_shared<SchedulerWorker>(i));  // Create workers (CPU cores)
    }
}

// Add a process to the queue for scheduling
void GlobalScheduler::scheduleProcess(std::shared_ptr<Process> process) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        processQueue.push(process);
    }
    processAvailable.notify_one();  // Notify the scheduler thread a new process is available
    //std::cout << "Process \"" << process->getName() << "\" added to scheduler queue." << std::endl;
}

// Start the scheduler in thread
void GlobalScheduler::start() {
    // Start all worker threads
    for (auto& worker : workers) {
        worker->start();  // Start each worker in each thread
    }
    
    stopRequested = false;
    // Start the scheduler in a separate thread to manage process dispatching
    schedulerThread = std::thread(&GlobalScheduler::dispatchProcesses, this);
}

// Scheduler logic for dispatching processes to workers
void GlobalScheduler::dispatchProcesses() {
    int maxCores = getConfigNumCPU();  // Use the configured number of CPU cores

    while (!stopRequested) {
        std::unique_lock<std::mutex> lock(queueMutex);
        processAvailable.wait(lock, [this] { return !processQueue.empty() || stopRequested; });

        if (stopRequested) break;

        while (!processQueue.empty()) {
            int activeCores = std::count_if(workers.begin(), workers.end(), [](const auto& worker) {
                return worker->isBusy();
                });

            if (activeCores >= maxCores) {
                std::this_thread::yield();  // Yield if all cores are busy
                break;
            }

            for (auto& worker : workers) {
                if (!worker->isBusy() && !processQueue.empty()) {
                    auto process = processQueue.front();
                    processQueue.pop();  // Remove from queue
                    worker->assignProcess(process);
                    worker->notify();  // Notify the worker to start processing

                    // Limit dispatching to available cores
                    if (++activeCores >= maxCores) break;
                }
            }

            std::this_thread::yield();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Small delay to avoid tight looping
    }
}

void GlobalScheduler::dispatchFCFS() {
    // Standard FCFS scheduling: assign processes to available workers as they arrive
    while (!processQueue.empty()) {
        bool processAssigned = false;

        for (auto& worker : workers) {
            if (!worker->isBusy() && !processQueue.empty()) {
                auto process = processQueue.front();
                processQueue.pop();

                worker->assignProcess(process);
                worker->notify();  // Notify the worker thread to start processing
                processAssigned = true;
            }
        }

        if (!processAssigned) {
            std::this_thread::yield();
            break;
        }
    }
}

void GlobalScheduler::dispatchRoundRobin() {
    while (!processQueue.empty()) {
        bool processAssigned = false;

        for (auto& worker : workers) {
            if (!worker->isBusy() && !processQueue.empty()) {
                auto process = processQueue.front();
                processQueue.pop();

                // Execute for a quantum of time (quantumCycles)
                process->setRemainingQuantum(quantumCycles);
                worker->assignProcess(process);
                worker->notify();
                processAssigned = true;
            }
        }

        if (!processAssigned) {
            std::this_thread::yield();
            break;
        }
    }
}

// Stop the scheduler and all worker threads
void GlobalScheduler::stop() {
    stopRequested = true;  // Signal the scheduler thread to stop
    processAvailable.notify_all();  // Wake up the scheduler thread if it’s waiting

    if (schedulerThread.joinable()) {
        schedulerThread.join();  // Ensure the scheduler thread is properly joined
    }

    for (auto& worker : workers) {
        worker->stop();  // Stop each worker thread
    }
}

//Can be removed later, debugging purposes
int GlobalScheduler::getQueueSize() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return processQueue.size();
}

void GlobalScheduler::startTestMode() {
    testModeActive = true;

    // Separate thread to generate dummy processes
    testThread = std::thread([this]() {
        int processCount = 0;
        int batchFrequency = getConfigBatchProcessFreq();
        int minInstructions = getConfigMinIns();
        int maxInstructions = getConfigMaxIns();

        while (testModeActive) {
            std::this_thread::sleep_for(std::chrono::milliseconds(batchFrequency * 100)); // Wait based on batch frequency

            // Create a new batch of processes
            std::string processName = "p" + std::to_string(processCount++);
            auto newProcess = std::make_shared<Process>(processName, processCount);

            // Generate random number of instructions between min and max
            int instructionCount = minInstructions + (std::rand() % (maxInstructions - minInstructions + 1));
            newProcess->populatePrintCommands(instructionCount);

            // Register the process with ConsoleManager without switching screens
            BaseScreen newScreen(newProcess, processName);
            ConsoleManager::getInstance()->registerScreen(std::make_shared<BaseScreen>(newScreen), false);

            this->scheduleProcess(newProcess);
            //std::cout << "Scheduler Queue Size After Adding: " << this->getQueueSize() << std::endl;
            //std::cout << "Generated dummy process: " << processName << " with " << instructionCount << " instructions." << std::endl;
        }
     });
}

void GlobalScheduler::stopTestMode() {
    testModeActive = false;
    if (testThread.joinable()) {
        testThread.join(); // Wait for the test thread to finish
    }
    std::cout << "Process generation stopped." << std::endl;
}
