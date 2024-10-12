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
        // Wait for a process to be assigned
        cv.wait(lock, [this] { return assignedProcess || !running; });

        if (assignedProcess) {
            // Debug: Log which worker (core) is starting to process which task
            // std::cout << "Worker " << workerId << " starting process " << assignedProcess->getName() << "\n" << std::endl;

            // Set the process start details
            assignedProcess->setCPUCoreID(workerId);
            assignedProcess->setTimeStarted();
            assignedProcess->setState(Process::RUNNING);

            // Execute all commands in the process
            while (assignedProcess->getCommandCounter() < assignedProcess->getTotalCommandCounter()) {
                assignedProcess->executeCurrentCommand();
                assignedProcess->moveToNextLine();
            }

            // Debug: Log when a process finishes
            // std::cout << "Worker " << workerId << " finished process " << assignedProcess->getName() << "\n" << std::endl;

            // Mark process as finished when done
            assignedProcess->setTimeFinished();
            assignedProcess->setState(Process::FINISHED);

            // Mark worker as no longer busy
            assignedProcess = nullptr;  // Clear the current process
            busy = false;  // Set worker to not busy
            cv.notify_all();  // Notify that the worker is available again
        }
    }

}

int SchedulerWorker::getWorkerId() {
    return workerId;
}