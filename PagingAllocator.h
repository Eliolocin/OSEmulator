#pragma once
#include "IMemoryAllocatorPaging.h"
#include <unordered_map>
#include <vector>
#include <string>
#include "Process.h"

class PagingAllocator : public IMemoryAllocatorPaging {
public:
    // Constructor: Initialize PagingAllocator with max memory size and frame size
    PagingAllocator(size_t maxMemorySize, size_t frameSize);

    // Allocate memory for a process
    void* allocatePaging(Process* process) override;

    // Deallocate memory for a process
    void deallocatePaging(Process* process) override;

    // Visualize the current memory allocation
    std::string visualizeMemoryPaging() override;

private:
    size_t maxMemorySize;                           // Total memory size
    size_t frameSize;                               // Frame size
    size_t numFrames;                               // Total number of frames
    std::vector<size_t> freeFrameList;              // List of free frames
    std::unordered_map<size_t, size_t> frameMap;    // Maps frame index to process ID

    // Helper methods
    size_t allocateFrames(size_t numFrames, size_t processId); // Allocate frames
    void deallocateFrames(size_t numFrames, size_t frameIndex); // Deallocate frames
};
