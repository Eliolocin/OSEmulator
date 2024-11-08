#include "GlobalScheduler.h"
#include "Config.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <queue>
#include "ConsoleManager.h"
#include "Utilities.h"

// Constructor initializes the number of workers (cores)
GlobalScheduler::GlobalScheduler(int numWorkers, IMemoryAllocator* memoryAllocator)
    : memoryAllocator(memoryAllocator), schedulerType(getConfigScheduler() == "rr" ? ROUND_ROBIN : FCFS),
    quantumCycles(getConfigQuantumCycles()) {
    for (int i = 0; i < numWorkers; ++i) {
        //std::cout << "CPU \"" << i << "\" added!" << std::endl;
        workers.push_back(std::make_shared<SchedulerWorker>(i, memoryAllocator));  // Create workers (CPU cores)
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
    int maxCores = getConfigNumCPU();

    while (!stopRequested) {
        std::unique_lock<std::mutex> lock(queueMutex);
        processAvailable.wait(lock, [this] { return !processQueue.empty() || stopRequested; });

        if (stopRequested) break;

        //std::cout << "[GlobalScheduler] Dispatching processes. Queue size: " << processQueue.size() << std::endl;

        while (!processQueue.empty()) {         
            int activeCores = std::count_if(workers.begin(), workers.end(), [](const auto& worker) {
                return worker->isBusy();
            });

            if (activeCores >= maxCores) {
                std::this_thread::yield();
                break;
            }
            for (auto& worker : workers) {
                //std::cout << "[GlobalScheduler] Checking worker " << worker->getWorkerId() << " status: " << (worker->isBusy() ? "busy" : "available") << std::endl;
                if (!worker->isBusy() && !processQueue.empty()) {
                    auto process = processQueue.front();
                    processQueue.pop();
                    
                    //std::cout << "[GlobalScheduler] Assigning process " << process->getName() << " to worker " << worker->getWorkerId() << std::endl;

                    worker->assignProcess(process);
                    worker->notify();

                    if (++activeCores >= maxCores) break;
                }
            }
            std::this_thread::yield();
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Stop the scheduler and all worker threads
void GlobalScheduler::stop() {
    stopRequested = true;  // Signal the scheduler thread to stop
    processAvailable.notify_all();  // Wake up the scheduler thread if it�s waiting

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
    //testThread = 
    std::thread([this]() {
        int processCount = 0;
        int batchFrequency = getConfigBatchProcessFreq();
        int minInstructions = getConfigMinIns();
        int maxInstructions = getConfigMaxIns();
        size_t minMemPerProcess = getConfigMinMemPerProcess();
		size_t maxMemPerProcess = getConfigMaxMemPerProcess();

        while (testModeActive) {
            if((getDelayCounter()-1) > 0)
            {
				setDelayCounter(getDelayCounter()-1);
			} // Do not create new process if delay counter is greater than 0
            else
            {
                // Restart delay counter and make new process if delay counter is 0 or less
                setDelayCounter(getConfigBatchProcessFreq());
                //std::this_thread::sleep_for(std::chrono::seconds(batchFrequency)); // Wait based on batch frequency

                // Create a new batch of processes
                std::string processName = "p" + std::to_string(processCount++);
                size_t processMemReq = randomMemSize(minMemPerProcess, maxMemPerProcess);
            	auto newProcess = std::make_shared<Process>(processName, processCount, processMemReq);//set to maxMem for fixed, change to processMemReq 

                // Generate random number of instructions between min and max
                int instructionCount = randomNumber(minInstructions, maxInstructions);
                newProcess->populatePrintCommands(instructionCount);

                // Register the process with ConsoleManager without switching screens
                BaseScreen newScreen(newProcess, processName);
                ConsoleManager::getInstance()->registerScreen(std::make_shared<BaseScreen>(newScreen), false);

                this->scheduleProcess(newProcess);
                //std::cout << "Scheduler Queue Size After Adding: " << this->getQueueSize() << std::endl;
                //std::cout << "Generated dummy process: " << processName << " with " << instructionCount << " instructions." << std::endl;
            }
            
        }
     }).detach(); // Detach the thread
}

void GlobalScheduler::stopTestMode() {
    /*testModeActive = false;
    if (testThread.joinable()) {
        testThread.join(); // Wait for the test thread to finish
    }
    std::cout << "Process generation stopped." << std::endl;*/
    testModeActive = false;
    std::cout << "Process generation stopped." << std::endl;
}

// Notify that a worker has become free and ready for assignment
void GlobalScheduler::notifyWorkerFree() {
    std::unique_lock<std::mutex> lock(queueMutex);
    if (!processQueue.empty()) {
        processAvailable.notify_one();  // Trigger dispatch if there are processes waiting
    }
}

int GlobalScheduler::getDelayCounter()
{
	return this->delayCounter;
}

void GlobalScheduler::setDelayCounter(int remainingDelay)
{
	this->delayCounter = remainingDelay;
}
