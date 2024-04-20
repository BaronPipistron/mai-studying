#include <gtest/gtest.h>
#include "../include/figures/Point.h"
#include "../src/figures/Point.cpp"

using namespace figures;

TEST(constructor_test, without_exception_test_1) {
    ASSERT_NO_THROW(Point<double>(0, 0));
}

TEST(constructor_test, without_exception_test_2) {
    ASSERT_NO_THROW(Point<double>(1.4, 10.8));
}

TEST(initializer_list_constructor_test, exception_test_1) {
    ASSERT_THROW(Point<double> { 0 }, std::invalid_argument);
}

TEST(initializer_list_constructor_test, exception_test_2) {
    ASSERT_THROW(Point<double> { 15.545 }, std::invalid_argument);
}

TEST(constructor_test, other_point_test_1) {
    Point<double> test_1 {4, 5.2};
    Point<double> test_2 (test_1);

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), test_2.get_x_cord());
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), test_2.get_y_cord());
}

TEST(constructor_test, other_point_test_2) {
    Point<double> test_1 {2.28, 1.337};
    Point<double> test_2 (test_1);

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), test_2.get_x_cord());
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), test_2.get_y_cord());
}

TEST(constructor_test, other_point_move_test_1) {
    Point<double> test_1 {2.341, 43};
    Point<double> test_2 (std::move(test_1));

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), test_2.get_x_cord());
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), test_2.get_y_cord());
}

TEST(constructor_test, other_point_move_test_2) {
    Point<double> test_1 {4313.3, 454.53};
    Point<double> test_2 (std::move(test_1));

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), test_2.get_x_cord());
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), test_2.get_y_cord());
}

TEST(assignment_operator_test, copy_assignment_test_1) {
    Point<double> test_1 {4, 5.2};
    Point<double> test_2 = test_1;

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), test_2.get_x_cord());
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), test_2.get_y_cord());
}

TEST(assignment_operator_test, copy_assignment_test_2) {
    Point<double> test_1 {4313.3, 454.53};
    Point<double> test_2 = test_1;

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), test_2.get_x_cord());
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), test_2.get_y_cord());
}

TEST(assignment_operator_test, move_assignment_test_1) {
    Point<double> test_1 {2.341, 43};
    Point<double> test_2 = std::move(test_1);

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), test_2.get_x_cord());
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), test_2.get_y_cord());
}

TEST(assignment_operator_test, move_assignment_test_2) {
    Point<double> test_1 {4313.3, 454.53};
    Point<double> test_2 = std::move(test_1);

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), test_2.get_x_cord());
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), test_2.get_y_cord());
}

TEST(get_cord_test, get_cord_test_1) {
    Point<double> test_1 {134.5, 42.2};

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), 134.5);
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), 42.2);
}

TEST(get_cord_test, get_cord_test_2) {
    Point<double> test_1 {224.54, 413};

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), 224.54);
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), 413);
}

TEST(set_cord_test, set_cord_test_1) {
    Point<double> test_1 {0, 0};

    test_1.set_x_cord(15);
    test_1.set_y_cord(20);

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), 15);
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), 20);
}

TEST(set_cord_test, set_cord_test_2) {
    Point<double> test_1 {15, 20};

    test_1.set_x_cord(0);
    test_1.set_y_cord(0);

    EXPECT_DOUBLE_EQ(test_1.get_x_cord(), 0);
    EXPECT_DOUBLE_EQ(test_1.get_y_cord(), 0);
}

TEST(equal_operator_test, true_return_1) {
    Point<double> test_1 (0, 0);
    Point<double> test_2 (0, 0);

    EXPECT_TRUE(test_1 == test_2);
}

TEST(equal_operator_test, true_return_2) {
    Point<double> test_1 (1, 54);
    Point<double> test_2 (1, 54);

    EXPECT_TRUE(test_1 == test_2);
}

TEST(equal_operator_test, false_return_1) {
    Point<double> test_1 (0, 0);
    Point<double> test_2 (1, 1);

    EXPECT_FALSE(test_1 == test_2);
}

TEST(equal_operator_test, false_return_2) {
    Point<double> test_1 (34, 45.4);
    Point<double> test_2 (4304.43, 41);

    EXPECT_FALSE(test_1 == test_2);
}

TEST(non_equal_operator_test, true_return_1) {
    Point<double> test_1 (0, 0);
    Point<double> test_2 (2, 2);

    EXPECT_TRUE(test_1 != test_2);
}

TEST(non_equal_operator_test, true_return_2) {
    Point<double> test_1 (44, 54.542);
    Point<double> test_2 (41.556, 65.43);

    EXPECT_TRUE(test_1 != test_2);
}

TEST(non_equal_operator_test, false_return_1) {
    Point<double> test_1 (0, 0);
    Point<double> test_2 (0, 0);

    EXPECT_FALSE(test_1 != test_2);
}

TEST(non_equal_operator_test, false_return_2) {
    Point<double> test_1 (34.5, 45.4);
    Point<double> test_2 (34.5, 45.4);

    EXPECT_FALSE(test_1 != test_2);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}