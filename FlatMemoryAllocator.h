#pragma once
#include "IMemoryAllocator.h"
#include <vector>
#include <unordered_map>
#include <string>

class FlatMemoryAllocator : public IMemoryAllocator {
public:
    FlatMemoryAllocator(size_t maximumSize);
    ~FlatMemoryAllocator();

    void* allocate(size_t size) override;
    void deallocate(void* ptr) override;
    std::string visualizeMemory(int qq) override;

private:
    size_t maximumSize;
    size_t allocatedSize;
    std::vector<char> memory;
    std::unordered_map<size_t, bool> allocationMap;

    void initializeMemory();
    bool canAllocateAt(size_t index, size_t size) const;
    void allocateAt(size_t index, size_t size);
    void deallocateAt(size_t index, size_t size);

    std::unordered_map<size_t, size_t> allocationSizes;  // Maps starting index to allocated size
};
