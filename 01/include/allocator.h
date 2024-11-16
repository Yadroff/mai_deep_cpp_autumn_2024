#pragma once

#include "common.h"

class Allocator {
public:
    void MakeAllocator(size_t maxSize);
    BYTE* Alloc(size_t size);
    void Reset();
    ~Allocator();

private:
    BYTE* buffer = nullptr;
    size_t offset = 0;
    size_t buffSize = 0;
};