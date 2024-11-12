#include "FlatMemoryAllocator.h"
#include "IMemoryAllocator.h"
#include "Process.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "ConsoleManager.h"
#include "BaseScreen.h"

// Constructor to initialize the memory allocator with a specified maximum size
FlatMemoryAllocator::FlatMemoryAllocator(size_t maximumSize)
    : maximumSize(maximumSize), allocatedSize(0) {
    memory.resize(maximumSize);  // Allocate and set the size
    initializeMemory(); // Initialize the memory contents
}

FlatMemoryAllocator::~FlatMemoryAllocator() {
    memory.clear();
}

// Function to allocate a contiguous block of memory of a specified size
void* FlatMemoryAllocator::allocate(size_t size) {
    // Find the first available contiguous block that can fit the requested size
    for (size_t i = 0; i <= maximumSize - size; ++i) {
        // Check if there's enough contiguous free space from position i
        if (!allocationMap[i] && canAllocateAt(i, size)) {
            // Allocate memory starting at index i if space is available
            allocateAt(i, size);
            return &memory[i]; // Return pointer to the beginning of allocated memory
        }
    }
    // No available block found, return nullptr
    //std::cout << "Failed to allocate memory of size " << size << "\n";
    return nullptr;
}

// Function to deallocate a previously allocated block of memory
void FlatMemoryAllocator::deallocate(void* ptr) {
    // Calculate the starting index in the memory vector from the given pointer
    size_t index = static_cast<char*>(ptr) - &memory[0];
    if (allocationMap[index]) { // Ensure the block was allocated
        // Find the size of the allocated block using the allocationSizes map
        auto it = allocationSizes.find(index);
        if (it != allocationSizes.end()) {
            size_t size = it->second; // Retrieve the size
            deallocateAt(index, size); // Deallocate the memory
            allocationSizes.erase(it);  // Remove entry from allocationSizes after deallocation
            //std::cout << "Deallocated memory at index " << index << " of size " << size << "\n";
        }
        else {
            std::cerr << "Warning: Attempted to deallocate untracked memory at index " << index << "\n";
        }
    }
    else {
        std::cerr << "Warning: Attempted to deallocate unallocated memory at index " << index << "\n";
    }
}

std::string FlatMemoryAllocator::visualizeMemory(int qq) {
    // Get all processes through their console
    std::vector<std::shared_ptr<BaseScreen>> processScreenList = ConsoleManager::getInstance()->getAllProcessScreens();

    // Gather memory information from all processes
    size_t totalMemory = getConfigMaxOverallMemory();
    size_t externalFragmentation = totalMemory;
    struct MemoryBlock {
        size_t startOffset; // Offset representation instead of address
        size_t size;
        std::string processName;
    };
    std::vector<MemoryBlock> memoryBlocks; // Stores memory blocks and their process IDs

    std::ostringstream textBuffer; // Buffer to store all output

    size_t currentOffset = 0; // Track current memory offset for each process
    for (size_t i = 0; i < processScreenList.size(); i++) {
        std::shared_ptr<BaseScreen> screen = processScreenList[i];
        std::shared_ptr<Process> process = screen->getProcess();
        if (process && process->isMemoryAllocated()) {
            size_t size = process->getMemoryRequired();
            std::string processName = process->getName();
            memoryBlocks.push_back({ currentOffset, size, processName });
            externalFragmentation -= size;
            currentOffset += size; // Increment offset by the size of the current process
        }
    }

    // Get current time for filename and timestamp
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    char timestamp[100];
    std::strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S%p", &localTime);

    // Write the timestamp
    textBuffer << "Timestamp: (" << timestamp << ")\n";

    // Write number of processes in memory
    textBuffer << "Number of processes in memory: " << memoryBlocks.size() << "\n";

    // Write total external fragmentation
    textBuffer << "Total external fragmentation in KB: " << externalFragmentation << "\n\n";

    // Visualize memory content from top to bottom
    textBuffer << "----end---- = " << totalMemory << "\n";
    size_t currentAddress = totalMemory;
    for (const auto& block : memoryBlocks) {
        size_t startAddress = currentAddress - block.size;

        textBuffer << "\n" << startAddress + block.size << "\n"; // End address of the block
        textBuffer << block.processName << "\n";
        textBuffer << startAddress << "\n";
        currentAddress = startAddress;
    }

    textBuffer << "\n----start---- = 0\n";

    // Ensure the directory exists
    std::filesystem::create_directories("./memoryCheck");
    for (char& c : timestamp) {
        if (c == '/' || c == ':' || c == ' ') {
            c = '_'; // Replace invalid characters with underscores
        }
    }

    // Write everything to the file at the end
    std::string qqString = std::to_string(qq);
    std::string filename = "./memoryCheck/memory_stamp_" + qqString + ".txt";
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << textBuffer.str();
        outFile.close();
    }
    else {
        throw std::runtime_error("Failed to create memory visualization file");
    }

    return filename;
}





// Initialize memory contents and allocation map to represent unallocated memory
void FlatMemoryAllocator::initializeMemory() {
    // Initialize memory vector with '.'
    std::fill(memory.begin(), memory.end(), '.');
    // Set each entry in allocationMap to false (unallocated)
    for (size_t i = 0; i < maximumSize; ++i) {
        allocationMap[i] = false;
    }
}

// Function to check if a contiguous block of memory starting at a given index can accommodate the requested size
bool FlatMemoryAllocator::canAllocateAt(size_t index, size_t size) const {
    // If requested block extends beyond maximum size, return false
    if (index + size > maximumSize) return false;  // Out of bounds
    
    // Verify that each byte in the requested range is unallocated
    for (size_t j = 0; j < size; ++j) {
        if (allocationMap.at(index + j)) {
            return false;  // Block is not fully free
        }
    }
    return true; // Block is available for allocation
    //return (index + size <= maximumSize);
}

// Function to allocate memory at a specified index with the requested size
void FlatMemoryAllocator::allocateAt(size_t index, size_t size) {
    // Mark each byte in the specified range as allocated
    for (size_t j = 0; j < size; ++j) {
        allocationMap[index + j] = true; // Mark as allocated in the map
        memory[index + j] = 'P';  // Visual indicator of allocation
    }
    allocationSizes[index] = size;  // Record allocation size at starting index
    allocatedSize += size;  // Update total allocated size
}

// Function to deallocate a block of memory starting at a given index and spanning a specified size
void FlatMemoryAllocator::deallocateAt(size_t index, size_t size) {
    // Reset each byte in the specified range to represent free memory
    for (size_t i = index; i < index + size; ++i) {
        allocationMap[i] = false; // Mark as unallocated in the map
        memory[i] = '.';  // Reset in the memory vector for visualization
    }
    allocatedSize -= size;
}
