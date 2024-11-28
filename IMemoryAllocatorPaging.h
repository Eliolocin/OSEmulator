#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "Process.h"

class IMemoryAllocatorPaging {
public:
    virtual void* allocatePaging(Process* process) = 0;
    virtual void deallocatePaging(Process* process) = 0;
    virtual std::string visualizeMemoryPaging() = 0;
};