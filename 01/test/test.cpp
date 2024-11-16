#include <gtest/gtest.h>

#include "allocator.h"
#include "common.h"

TEST(AllocTests, Byte)
{
    Allocator alloc;
    alloc.MakeAllocator(sizeof(BYTE));
    BYTE* byte = nullptr;
    ASSERT_NO_THROW(byte = alloc.Alloc(sizeof(BYTE)));
    ASSERT_NE(byte, nullptr);
}

TEST(AllocTests, Int)
{
    Allocator alloc;
    alloc.MakeAllocator(sizeof(int));
    int* ptr = nullptr;
    ASSERT_NO_THROW(ptr = reinterpret_cast<int*>(alloc.Alloc(sizeof(int))));
    ASSERT_NE(ptr, nullptr);
}

TEST(ResetTests, BeforeResetEqAfterReset)
{
    Allocator alloc;
    alloc.MakeAllocator(sizeof(BYTE));
    BYTE* beforeReset = nullptr;
    ASSERT_NO_THROW(beforeReset = alloc.Alloc(sizeof(BYTE)));
    alloc.Reset();
    BYTE* afterReset = nullptr;
    ASSERT_NO_THROW(afterReset = alloc.Alloc(sizeof(BYTE)));
    ASSERT_EQ(beforeReset, afterReset);
}

TEST(ResetTests, UseAfterReset)
{
    Allocator alloc;
    alloc.MakeAllocator(sizeof(int));
    int* beforeReset = reinterpret_cast<int*>(alloc.Alloc(sizeof(int)));
    *beforeReset = 42;
    alloc.Reset();
    int* afterReset = reinterpret_cast<int*>(alloc.Alloc(sizeof(int)));
    ASSERT_EQ(beforeReset, afterReset);
    ASSERT_EQ(*beforeReset, *afterReset);
    *afterReset = 0;
    ASSERT_EQ(*beforeReset, *afterReset);
}

TEST(MakeAllocator, MakeNewAlloc)
{
    Allocator alloc;
    alloc.MakeAllocator(sizeof(BYTE));
    alloc.MakeAllocator(sizeof(int));
}

TEST(MakeAllocator, MakeAndAlloc)
{
    Allocator alloc;
    alloc.MakeAllocator(sizeof(BYTE));
    alloc.MakeAllocator(sizeof(int));
    int* ptr = nullptr;
    ASSERT_NO_THROW(ptr = reinterpret_cast<int*>(alloc.Alloc(sizeof(int))));
    ASSERT_NE(ptr, nullptr);
}

TEST(AllocatorTests_Death, EmptyBuff)
{
    Allocator alloc;
    ASSERT_THROW(alloc.Alloc(sizeof(BYTE)), std::bad_alloc);
}

TEST(AllocatorTests_Death, BigAlloc)
{
    Allocator alloc;
    alloc.MakeAllocator(sizeof(BYTE));
    ASSERT_THROW(alloc.Alloc(sizeof(int)), std::bad_alloc);
}

TEST(AllocatorTests_Death, NegativeSize)
{
    Allocator alloc;
    alloc.MakeAllocator(sizeof(BYTE));
    ASSERT_THROW(alloc.Alloc(static_cast<size_t>(-1)), std::bad_alloc);
}

TEST(AllocatorTests_Death, UseAfterRealloc)
{
    Allocator alloc;
    alloc.MakeAllocator(sizeof(BYTE));
    BYTE* ptr = alloc.Alloc(sizeof(BYTE));
    alloc.MakeAllocator(sizeof(int));
    ASSERT_DEATH(BYTE byte = *ptr, "");
}

TEST(AllocatorTests_Death, BadAlloc)
{
    Allocator alloc;
    alloc.MakeAllocator(sizeof(int)+sizeof(BYTE));
    int* ptr = reinterpret_cast<int*>(alloc.Alloc(sizeof(int)));
    ASSERT_NE(ptr, nullptr);
    ASSERT_ANY_THROW(alloc.Alloc(sizeof(int)));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}