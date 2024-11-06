#include "SchedulerWorker.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <memory>
#include <iomanip>
#include "Commands.h"	
#include "Utilities.h"
#include "TypeDefs.h"
#include <sstream>
#include <ctime>
#include "GlobalScheduler.h"
#include "ConsoleManager.h"
#include "Config.h"


SchedulerWorker::SchedulerWorker(int id) : workerId(id), running(false), busy(false) {}

void SchedulerWorker::assignProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(mtx);
    assignedProcess = process;
    busy = true;
    cv.notify_one();
}

void SchedulerWorker::start() {
    running = true;
    workerThread = std::thread(&SchedulerWorker::processExecution, this);
    workerThread.detach();  // Worker threads are detached
}

void SchedulerWorker::stop() {
    running = false;
    cv.notify_one();  // Wake up the thread if it's waiting
}

bool SchedulerWorker::isBusy() const {
    return busy;
}

void SchedulerWorker::notify() {
    cv.notify_one();  // Notify the condition variable to wake up the worker
}


void SchedulerWorker::processExecution() {
    /*while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return assignedProcess || !running; });

        if (assignedProcess) {
            assignedProcess->setCPUCoreID(workerId);
            assignedProcess->setTimeStarted();
            assignedProcess->setState(Process::RUNNING);

            // Reset the process quantum for Round Robin
            int quantumCycles = getConfigQuantumCycles();  // Fetch quantum from config
            assignedProcess->resetQuantum(quantumCycles);

            while (assignedProcess->getCommandCounter() < assignedProcess->getTotalCommandCounter()) {
                // Execute one command and decrement quantum
                assignedProcess->executeCurrentCommand();
                assignedProcess->moveToNextLine();
                assignedProcess->decrementQuantum();

                // If quantum expires, pause execution and requeue
                if (assignedProcess->getRemainingQuantum() <= 0) {
                    assignedProcess->setState(Process::READY);  // Set back to READY
                    ConsoleManager::getInstance()->getGlobalScheduler()->scheduleProcess(assignedProcess);  // Re-queue
                    assignedProcess = nullptr;
                    busy = false;
                    cv.notify_all();
                    break;
                }
            }

            // If process finished all commands, mark it as finished
            if (assignedProcess && assignedProcess->isFinished()) {
                assignedProcess->setTimeFinished();
                assignedProcess->setState(Process::FINISHED);
                assignedProcess = nullptr;
                busy = false;
                cv.notify_all();
            }
        }
    }*/
    while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return assignedProcess || !running; });

        if (!running) break;

        if (assignedProcess) {
            //std::cout << "[Worker " << workerId << "] Starting process " << assignedProcess->getName() << std::endl;
            assignedProcess->setCPUCoreID(workerId);
            assignedProcess->setTimeStarted();
            assignedProcess->setState(Process::RUNNING);

            // Fetch the scheduler type from the configuration
            std::string schedulerType = getConfigScheduler();

            if (schedulerType == "fcfs") {
                // FCFS: Execute the entire process without interruption
                while (assignedProcess->getCommandCounter() < assignedProcess->getTotalCommandCounter()) {

					if (assignedProcess->getDelayCounter() > 0) {
						assignedProcess->setDelayCounter(assignedProcess->getDelayCounter() - 1);
						continue; // Skip iteration if delay is not 0
					}
                    // Reset delay counter and execute command if delay is 0
					assignedProcess->setDelayCounter(getConfigDelayPerExec()); 

                    assignedProcess->executeCurrentCommand();
                    assignedProcess->moveToNextLine();
                }

                // Process is finished
                assignedProcess->setTimeFinished();
                assignedProcess->setState(Process::FINISHED);
                assignedProcess = nullptr;
                busy = false;
                cv.notify_all();

            }
            else if (schedulerType == "rr") {
                // Reset the process quantum for Round Robin
                int quantumCycles = getConfigQuantumCycles();  // Fetch quantum from config
                assignedProcess->resetQuantum(quantumCycles);
				assignedProcess->setDelayCounter(getConfigDelayPerExec()); // Reset delay counter

                while (assignedProcess->getCommandCounter() < assignedProcess->getTotalCommandCounter()) {
                    //std::cout << assignedProcess->getRemainingQuantum() << " Remaining Quantum " << std::endl;

                    // 0 0
                    // If quantum expires, pause execution and requeue
                    if (assignedProcess&&(assignedProcess->getRemainingQuantum()-1)<= -1) {
						//std::cout << "[Worker " << workerId << "] Quantum expired for process " << assignedProcess->getName() << ". Re-queuing." << std::endl;
                        assignedProcess->setState(Process::READY);  // Set back to READY
                        ConsoleManager::getInstance()->getGlobalScheduler()->scheduleProcess(assignedProcess);  // Re-queue
                        assignedProcess = nullptr;
                        busy = false;
                        cv.notify_all();
                        break;
                    }

                    // Execute one command and decrement quantum
                    if ((assignedProcess->getDelayCounter()-1) > 0) {
                        assignedProcess->decrementQuantum();
                        // -1 0
                        assignedProcess->setDelayCounter(assignedProcess->getDelayCounter() - 1);
                        // 0 0
                        continue; // Skip iteration if delay is not 0
                    }
                    // Reset delay counter and execute command if delay is 0
                    assignedProcess->decrementQuantum();
                    assignedProcess->setDelayCounter(getConfigDelayPerExec());

                    assignedProcess->executeCurrentCommand();
                    assignedProcess->moveToNextLine();
                    
                }

                // If process finished all commands, mark it as finished
                if (assignedProcess && assignedProcess->isFinished()) {
                    assignedProcess->setTimeFinished();
                    assignedProcess->setState(Process::FINISHED);
                    assignedProcess = nullptr;
                    busy = false;
                    cv.notify_all();
                    //ConsoleManager::getInstance()->getGlobalScheduler()->notifyWorkerFree();
                }
            }

            // Clear the current process and set worker to not busy
            //assignedProcess = nullptr;
            //busy = false;
            //busy.store(false);
            //cv.notify_all();
            // Notify scheduler that the worker is free to take a new process
            //ConsoleManager::getInstance()->getGlobalScheduler()->notifyWorkerFree();
            //std::cout << "[Worker " << workerId << "] Available for new process. Notifying scheduler." << std::endl;
        }
    }
}

int SchedulerWorker::getWorkerId() {
    return workerId;
}