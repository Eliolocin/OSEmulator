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
}

void SchedulerWorker::stop() {
    running = false;
    cv.notify_one();  // Wake up the thread if it's waiting
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

bool SchedulerWorker::isBusy() const {
    return busy;
}

void SchedulerWorker::notify() {
    cv.notify_one();  // Notify the condition variable to wake up the worker
}


void SchedulerWorker::processExecution() {
    while (running) {
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
    }
}

int SchedulerWorker::getWorkerId() {
    return workerId;
}