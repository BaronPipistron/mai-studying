#include <gtest/gtest.h>
#include "../include/task.hpp"

using namespace MAI::OOP::Lab1;

TEST(task_func_test, basic_test_set)
{
    ASSERT_TRUE(Solution::task(1) == "00000000000000000000000000000001");
}

TEST(task_func_test, param_zero_test_set)
{
    ASSERT_TRUE(Solution::task(0) == "00000000000000000000000000000000");
}

TEST(task_func_test, random_param_test_set)
{
    ASSERT_TRUE(Solution::task(14) == "00000000000000000000000000001110");
}

TEST(task_func_test, max_32_bit_uint_test_set)
{
    ASSERT_TRUE(Solution::task(4294967295) == "11111111111111111111111111111111");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}