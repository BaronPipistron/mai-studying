#include <gtest/gtest.h>
#include <type_traits>

#include "../include/allocator/Allocator.h"
#include "../src/allocator/Allocator.cpp"

using namespace allocator;

TEST(allocator_test_set, inner_types_is_convertible) {
    bool p2constP = std::is_convertible_v<Allocator<int>::pointer,
                                         Allocator<int>::const_pointer>;
   

    ASSERT_TRUE(p2constP);
}

TEST(AllocatorTestSet, allocateTest) {
    Allocator<int, 2> allocator;

    int* pint = allocator.allocate(1);

    EXPECT_NE(pint, nullptr);
    EXPECT_THROW(allocator.allocate(999999), std::bad_alloc);

    allocator.deallocate(pint, 1);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}