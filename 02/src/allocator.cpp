#include <exception>

#include "allocator.h"

void Allocator::MakeAllocator(size_t maxSize)
{
    delete buffer; // nothing will happen if buffer is nullptr
    buffer = new BYTE[maxSize];
    buffSize = maxSize;
}

BYTE* Allocator::Alloc(size_t size)
{
    if (offset + size > buffSize) {
        throw std::bad_alloc();
    }
    return buffer + offset;
}

void Allocator::Reset()
{
    offset = 0;
}

Allocator::~Allocator()
{
    delete buffer;
}
