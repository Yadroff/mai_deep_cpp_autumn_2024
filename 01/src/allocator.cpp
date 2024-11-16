#include <exception>
#include <new>

#include "allocator.h"

void Allocator::MakeAllocator(size_t maxSize)
{
    delete[] buffer; // nothing will happen if buffer is nullptr
    buffer = new BYTE[maxSize];
    buffSize = maxSize;
}

BYTE* Allocator::Alloc(size_t size)
{
    if (offset+size>buffSize) {
        throw std::bad_alloc();
    }
    BYTE* ret = buffer + offset;
    offset += size;
    return ret;
}

void Allocator::Reset()
{
    offset = 0;
}

Allocator::~Allocator()
{
    delete[] buffer;
}
