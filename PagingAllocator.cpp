#include "PagingAllocator.h"
#include <iostream>
#include <sstream>
#include <algorithm> // For std::sort

// Constructor: Initialize Paging Allocator
PagingAllocator::PagingAllocator(size_t maxMemorySize, size_t frameSize)
    : maxMemorySize(maxMemorySize), frameSize(frameSize) {
    numFrames = maxMemorySize / frameSize; // Calculate total number of frames
    for (size_t i = 0; i < numFrames; ++i) {
        freeFrameList.push_back(i); // Populate free frame list
    }
}

// Allocate memory for a process
void* PagingAllocator::allocatePaging(Process* process) {
    size_t processId = process->getPid();
    size_t numFramesNeeded = process->getNumFramesNeeded();

    //DEBUG LINES PAGING:
    //std::cout << "Current free frames: ";
    //for (const auto& frame : freeFrameList) {
        //std::cout << frame << " ";
    //}
    //std::cout << std::endl;
    //std::cout << "Allocating Paging: PID=" << process->getPid() << ", FramesNeeded=" << numFramesNeeded << ", FreeFrames=" << freeFrameList.size() << std::endl;


    if (numFramesNeeded > freeFrameList.size()) {
        //DEBUG LINES PAGING:
        //std::cerr << "Memory allocation failed for process " << processId << ". Not enough free frames.\n";
        return nullptr;
    }

    size_t frameIndex = allocateFrames(numFramesNeeded, processId);
    return reinterpret_cast<void*>(frameIndex); // Return starting frame index as a pointer
}

// Deallocate memory for a process
void PagingAllocator::deallocatePaging(Process* process) {
    size_t processId = process->getPid();
    //DEBUG LINES PAGING:
    //std::cout << "Deallocating memory for PID=" << processId << std::endl;

    // Find all frames allocated to this process
    auto it = std::find_if(frameMap.begin(), frameMap.end(),
        [processId](const auto& entry) { return entry.second == processId; });

    while (it != frameMap.end()) {
        size_t frameIndex = it->first;
        //DEBUG LINES PAGING:
        //std::cout << "Deallocating frame " << frameIndex << " for PID=" << processId << std::endl;

        frameMap.erase(it); // Remove the frame allocation

        freeFrameList.push_back(frameIndex); // Add the frame back to the free list
        incOutPages();

        // Find the next frame belonging to this process
        it = std::find_if(frameMap.begin(), frameMap.end(),
            [processId](const auto& entry) { return entry.second == processId; });
    }

    //DEBUG LINES PAGING:
    //std::cout << "Deallocation complete. FreeFrameList size: " << freeFrameList.size() << std::endl;
    //std::cout << "Current free frames after deallocation: ";
    
    std::sort(freeFrameList.begin(), freeFrameList.end());  // Sort freeFrameList to maintain sequential order

    //DEBUG LINES PAGING:
    //for (const auto& frame : freeFrameList) {
        //std::cout << frame << " ";
    //}
    //std::cout << std::endl;
}


// Visualize the current memory state
std::string PagingAllocator::visualizeMemoryPaging() {
    std::ostringstream oss;
    oss << "Memory Visualization:\n";
    for (size_t frameIndex = 0; frameIndex < numFrames; ++frameIndex) {
        auto it = frameMap.find(frameIndex);
        if (it != frameMap.end()) {
            oss << "Frame " << frameIndex << " -> Process " << it->second << "\n";
        }
        else {
            oss << "Frame " << frameIndex << " -> Free\n";
        }
    }
    oss << "----------------------------------\n";
    return oss.str();
}

// Allocate a specific number of frames
size_t PagingAllocator::allocateFrames(size_t numFrames, size_t processId) {
    if (numFrames > freeFrameList.size()) {
        //DEBUG LINES PAGING:
        //std::cerr << "Error: Not enough free frames to allocate " << numFrames << " frames.\n";
        return -1; // Indicate failure
    }

    // Allocate the first frame and track the starting frame index
    size_t startFrame = freeFrameList.back();
    //DEBUG LINES PAGING:
    //std::cout << "startframe value: " << startFrame << std::endl;

    for (size_t i = 0; i < numFrames; ++i) {
        size_t frameIndex = freeFrameList.back(); // Take the last free frame
        frameMap[frameIndex] = processId;        // Map frame to process
        freeFrameList.pop_back();                // Remove from free list
        incInPages();
        //DEBUG LINES PAGING:
        //std::cout << "Allocated frame " << frameIndex << " to process " << processId << std::endl;
    }

    return startFrame; // Return the starting frame index
}



// Deallocate frames for a process
void PagingAllocator::deallocateFrames(size_t numFrames, size_t frameIndex) {
    for (size_t i = 0; i < numFrames; ++i) {
        frameMap.erase(frameIndex + i);         // Remove from frame map
        freeFrameList.push_back(frameIndex + i); // Return to free list
    }
}
