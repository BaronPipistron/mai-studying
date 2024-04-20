#include <gtest/gtest.h>

#include "../include/containers/Queue.h"
#include "../src/containers/Queue.cpp"

using namespace containers;

TEST(constructors_test_case, default_constructor_test) {
    Queue<int> test1;

    EXPECT_EQ(test1.size(), 0);
    EXPECT_TRUE(test1.empty());
}

TEST(constructors_test_case, initializer_list_test) {
    Queue<int> test1 = {1, 2, 3, 4, 5};

    EXPECT_EQ(test1.size(), 5);
    EXPECT_EQ(test1.front(), 1);
    EXPECT_EQ(test1.back(), 5);
}

TEST(basic_functions_case, front_test) {
    Queue<int> test1 = {1, 2, 3, 4, 5};

    EXPECT_EQ(test1.front(), 1);
}

TEST(basic_functions_case, back_test) {
    Queue<int> test1 = {1, 2, 3, 4, 5};

    EXPECT_EQ(test1.back(), 5);
}

TEST(basic_functions_case, empty_test_false_return) {
    Queue<int> test1 = {1, 2, 3, 4, 5};

    EXPECT_FALSE(test1.empty());
}

TEST(basic_functions_case, empty_test_true_return) {
    Queue<int> test1;

    EXPECT_TRUE(test1.empty());
}

TEST(basic_functions_case, size_test) {
    Queue<int> test1 = {1, 2, 3, 4, 5};

    EXPECT_EQ(test1.size(), 5);
}

TEST(basic_functions_case, clear_test) {
    Queue<int> test1 = {1, 2, 3, 4, 5};

    EXPECT_EQ(test1.size(), 5);
    EXPECT_EQ(test1.front(), 1);
    EXPECT_EQ(test1.back(), 5);

    test1.clear();

    EXPECT_EQ(test1.size(), 0);
}

TEST(insert_functions_case, push_test) {
    Queue<int> test1 = {1, 2, 3, 4, 5};
    EXPECT_EQ(test1.back(), 5);

    test1.push(6);
    EXPECT_EQ(test1.back(), 6);

    test1.push(7);
    EXPECT_EQ(test1.back(), 7);
}

TEST(insert_functions_case, pop_test) {
    Queue<int> test1 = {1, 2, 3, 4, 5};
    EXPECT_EQ(test1.front(), 1);

    test1.pop();
    EXPECT_EQ(test1.front(), 2);

    test1.pop();
    EXPECT_EQ(test1.front(), 3);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}