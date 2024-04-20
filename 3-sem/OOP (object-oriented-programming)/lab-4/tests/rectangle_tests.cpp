#include <gtest/gtest.h>
#include "../include/figures/Rectangle.h"
#include "../src/figures/Rectangle.cpp"
#include "../include/figures/Point.h"
#include "../src/figures/Point.cpp"

using namespace figures;

TEST(constructor_test, without_exception_test_1) {
    ASSERT_NO_THROW(
                    Rectangle<double>(
                                      Point<double>(0, 0),
                                      Point<double>(1, 0),
                                      Point<double>(0, 1),
                                      Point<double>(1, 1)
                                      )
                    );
}

TEST(constructor_test, without_exception_test_2) {
    ASSERT_NO_THROW(Rectangle<double>(
                                      Point<double>(1, 1),
                                      Point<double>(10, 1),
                                      Point<double>(1, 5),
                                      Point<double>(10, 5)
                                      )
                    );
}

TEST(constructor_test, with_exception_test_1) {
    ASSERT_THROW(
                 Rectangle<double>(
                                   Point<double>(0, 1),
                                   Point<double>(-1, 1),
                                   Point<double>(41.431, 5),
                                   Point<double>(0, -45)
                                   ), 
                 std::invalid_argument
                 );
}

TEST(constructor_test, with_exception_test_2) {
    ASSERT_THROW(
                 Rectangle<double>(
                                   Point<double>(3.4, 43),
                                   Point<double>(2, 78),
                                   Point<double>(-31, 5),
                                   Point<double>(34, 49)
                                   ),
                 std::invalid_argument
                 );
}

TEST(length_width_constructor_test, without_exception_test_1) {
    ASSERT_NO_THROW(Rectangle<double>(10, 4));
}

TEST(length_width_constructor_test, without_exception_test_2) {
    ASSERT_NO_THROW(Rectangle<double>(49, 32));
}

TEST(length_width_constructor_test, with_exception_test_1) {
    ASSERT_THROW(
                 Rectangle<double>(0, 0),
                 std::invalid_argument
                 );
}

TEST(length_width_constructor_test, with_exception_test_2) {
    ASSERT_THROW(
                 Rectangle<double>(-143, -43.5),
                 std::invalid_argument
                 );
}

TEST(constructor_test, other_rectangle_test) {
    Rectangle<double> test_1 (
                              Point<double>(1, 1),
                              Point<double>(10, 1),
                              Point<double>(1, 5),
                              Point<double>(10, 5)
                              );                   
    Rectangle<double> test_2 (test_1);

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_length(), test_2.get_length());
    EXPECT_DOUBLE_EQ(test_1.get_width(), test_2.get_width());
}

TEST(constructor_test, other_rectangle_move_test) {
    Rectangle<double> test_1 (
                              Point<double>(1, 1),
                              Point<double>(10, 1),
                              Point<double>(1, 5),
                              Point<double>(10, 5)
                              );
    Rectangle<double> test_2 (std::move(test_1));

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_length(), test_2.get_length());
    EXPECT_DOUBLE_EQ(test_1.get_width(), test_2.get_width());
}

TEST(assigment_operator_test, copy_assigment_test) {
    Rectangle<double> test_1 (
                              Point<double>(1, 1), 
                              Point<double>(10, 1), 
                              Point<double>(1, 5), 
                              Point<double>(10, 5)
                              );
    Rectangle<double> test_2 = test_1;

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_length(), test_2.get_length());
    EXPECT_DOUBLE_EQ(test_1.get_width(), test_2.get_width());
}

TEST(assigment_operator_test, move_assigment_test) {
    Rectangle<double> test_1 (
                              Point<double>(1, 1),
                              Point<double>(10, 1), 
                              Point<double>(1, 5), 
                              Point<double>(10, 5)
                              );
    Rectangle<double> test_2 = std::move(test_1);

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_length(), test_2.get_length());
    EXPECT_DOUBLE_EQ(test_1.get_width(), test_2.get_width());
}

TEST(calculate_geometric_center_test, calculate_geometric_center_test_1) {
    Rectangle<double> test_1 (
                              Point<double>(0, 0), 
                              Point<double>(2, 0), 
                              Point<double>(0, 2), 
                              Point<double>(2, 2)
                              );
    Point<double> center_test = test_1.calculate_geometric_center();

    EXPECT_DOUBLE_EQ(center_test.get_x_cord(), 1);
    EXPECT_DOUBLE_EQ(center_test.get_y_cord(), 1);
}

TEST(calculate_geometric_center_test, calculate_geometric_center_test_2) {
    Rectangle<double> test_1 (
                              Point<double>(1, 1), 
                              Point<double>(10, 1), 
                              Point<double>(1, 5), 
                              Point<double>(10, 5)
                              );
    Point<double> center_test = test_1.calculate_geometric_center();

    EXPECT_DOUBLE_EQ(center_test.get_x_cord(), 5.5);
    EXPECT_DOUBLE_EQ(center_test.get_y_cord(), 3);
}

TEST(calculate_perimeter_test, calculate_perimeter_test_1) {
    Rectangle<double> test_1 (
                              Point<double>(0, 0), 
                              Point<double>(2, 0), 
                              Point<double>(0, 2), 
                              Point<double>(2, 2)
                              );

    EXPECT_DOUBLE_EQ(test_1.calculate_perimeter(), 8);
}

TEST(calculate_perimeter_test, calculate_perimeter_test_2) {
    Rectangle<double> test_1 (5, 10);

    EXPECT_DOUBLE_EQ(test_1.calculate_perimeter(), 30);
}

TEST(calculate_area_test, calculate_area_test_1) {
    Rectangle<double> test_1 (
                              Point<double>(0, 0), 
                              Point<double>(2, 0), 
                              Point<double>(0, 2), 
                              Point<double>(2, 2)
                              );

    EXPECT_DOUBLE_EQ(test_1.calculate_area(), 4);
}

TEST(calculate_area_test, calculate_area_test_2) {
    Rectangle<double> test_1 (5, 10);

    EXPECT_DOUBLE_EQ(test_1.calculate_area(), 50);
}

TEST(get_lenth_test, get_length_test_1) {
    Rectangle<double> test_1 (
                              Point<double>(1, 1), 
                              Point<double>(10, 1), 
                              Point<double>(1, 5), 
                              Point<double>(10, 5)
                              );

    EXPECT_DOUBLE_EQ(test_1.get_length(), 9);
}

TEST(get_lenth_test, get_length_test_2) {
    Rectangle<double> test_1 (4.57, 2.28);

    EXPECT_DOUBLE_EQ(test_1.get_length(), 4.57);
}

TEST(get_width_test, get_width_test_1) {
    Rectangle<double> test_1 (
                              Point<double>(1, 1), 
                              Point<double>(10, 1), 
                              Point<double>(1, 5), 
                              Point<double>(10, 5)
                              );

    EXPECT_DOUBLE_EQ(test_1.get_width(), 4);
}

TEST(get_width_test, get_width_test_2) {
    Rectangle<double> test_1 (4.57, 2.28);

    EXPECT_DOUBLE_EQ(test_1.get_width(), 2.28);
}

TEST(operator_double_test, operator_double_test_1) {
    Rectangle<double> test_1 (
                              Point<double>(0, 0), 
                              Point<double>(2, 0), 
                              Point<double>(0, 2), 
                              Point<double>(2, 2)
                              );
    double operator_double_test_val = double(test_1);

    EXPECT_DOUBLE_EQ(operator_double_test_val, 4);
}

TEST(operator_double_test, operator_double_test_2) {
    Rectangle<double> test_1 (5, 10);
    double operator_double_test_val = double(test_1);

    EXPECT_DOUBLE_EQ(operator_double_test_val, 50);
}

TEST(equal_operator_test, true_return_1) {
    Rectangle<double> test_1 (
                              Point<double>(0, 0), 
                              Point<double>(2, 0), 
                              Point<double>(0, 2), 
                              Point<double>(2, 2)
                              );

    Rectangle<double> test_2 (
                              Point<double>(0, 0), 
                              Point<double>(2, 0), 
                              Point<double>(0, 2), 
                              Point<double>(2, 2)
                              );

    EXPECT_TRUE(test_1 == test_2);
}

TEST(equal_operator_test, true_return_2) {
    Rectangle<double> test_1 (5, 10);
    Rectangle<double> test_2 (5, 10);

    EXPECT_TRUE(test_1 == test_2);
}

TEST(equal_operator_test, false_return_1) {
    Rectangle<double> test_1 (
                              Point<double>(0, 0), 
                              Point<double>(2, 0), 
                              Point<double>(0, 2), 
                              Point<double>(2, 2)
                              );

    Rectangle<double> test_2 (
                              Point<double>(0, 0), 
                              Point<double>(4, 0), 
                              Point<double>(0, 4), 
                              Point<double>(4, 4)
                              );

    EXPECT_FALSE(test_1 == test_2);
}

TEST(equal_operator_test, false_return_2) {
    Rectangle<double> test_1 (6, 4.6);
    Rectangle<double> test_2 (2.56, 4);

    EXPECT_FALSE(test_1 == test_2);
}

TEST(non_equal_operator_test, true_return_1) {
    Rectangle<double> test_1 (
                              Point<double>(0, 0), 
                              Point<double>(2, 0), 
                              Point<double>(0, 2), 
                              Point<double>(2, 2)
                              );

    Rectangle<double> test_2 (
                              Point<double>(0, 0), 
                              Point<double>(4, 0), 
                              Point<double>(0, 4), 
                              Point<double>(4, 4)
                              );

    EXPECT_TRUE(test_1 != test_2);
}

TEST(non_equal_operator_test, true_return_2) {
    Rectangle<double> test_1 (6, 4.6);
    Rectangle<double> test_2 (2.56, 4);

    EXPECT_TRUE(test_1 != test_2);
}

TEST(non_equal_operator_test, false_return_1) {
    Rectangle<double> test_1 (
                              Point<double>(0, 0), 
                              Point<double>(2, 0), 
                              Point<double>(0, 2), 
                              Point<double>(2, 2)
                              );

    Rectangle<double> test_2 (
                              Point<double>(0, 0), 
                              Point<double>(2, 0), 
                              Point<double>(0, 2), 
                              Point<double>(2, 2)
                              );

    EXPECT_FALSE(test_1 != test_2);
}

TEST(non_equal_operator_test, false_return_2) {
    Rectangle<double> test_1 (5, 10);
    Rectangle<double> test_2 (5, 10);

    EXPECT_FALSE(test_1 != test_2);
}

TEST(rotated_rectangle_tests, copy_constructor_test) {
    Rectangle<double> test_1 (
                              Point<double>(2, 2), 
                              Point<double>(4, 1), 
                              Point<double>(3, 4), 
                              Point<double>(5, 3)
                              );
    Rectangle<double> test_2 (test_1);

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_length(), test_2.get_length());
    EXPECT_DOUBLE_EQ(test_1.get_width(), test_2.get_width());
}

TEST(rotated_rectangle_tests, move_constructor_test) {
    Rectangle<double> test_1 (
                              Point<double>(2, 2), 
                              Point<double>(4, 1), 
                              Point<double>(3, 4), 
                              Point<double>(5, 3)
                              );
    Rectangle<double> test_2 (std::move(test_1));

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_length(), test_2.get_length());
    EXPECT_DOUBLE_EQ(test_1.get_width(), test_2.get_width());
}

TEST(rotated_rectangle_tests, assigment_operator_copy_test) {
    Rectangle<double> test_1 (
                              Point<double>(2, 2), 
                              Point<double>(4, 1), 
                              Point<double>(3, 4), 
                              Point<double>(5, 3)
                              );
    Rectangle<double> test_2 = test_1;

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_length(), test_2.get_length());
    EXPECT_DOUBLE_EQ(test_1.get_width(), test_2.get_width());
}

TEST(rotated_rectangle_tests, assigment_operator_move_test) {
    Rectangle<double> test_1 (
                              Point<double>(2, 2), 
                              Point<double>(4, 1), 
                              Point<double>(3, 4), 
                              Point<double>(5, 3)
                              );
    Rectangle<double> test_2 = std::move(test_1);

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_length(), test_2.get_length());
    EXPECT_DOUBLE_EQ(test_1.get_width(), test_2.get_width());
}

TEST(rotated_rectangle_tests, calculate_geometric_center_test) {
    Rectangle<double> test_1 (
                              Point<double>(2, 2), 
                              Point<double>(4, 1), 
                              Point<double>(3, 4), 
                              Point<double>(5, 3)
                              );
    Point<double> center_test = test_1.calculate_geometric_center();

    EXPECT_DOUBLE_EQ(center_test.get_x_cord(), 3.5);
    EXPECT_DOUBLE_EQ(center_test.get_y_cord(), 2.5);
}

TEST(rotated_rectangle_tests, calculate_perimeter_test) {
    Rectangle<double> test_1 (
                              Point<double>(2, 2), 
                              Point<double>(4, 1), 
                              Point<double>(3, 4), 
                              Point<double>(5, 3)
                              );

    EXPECT_DOUBLE_EQ(test_1.calculate_perimeter(), sqrt(5) * 4);
}

TEST(rotated_rectangle_tests, calculate_area_test) {
    Rectangle<double> test_1 (
                              Point<double>(2, 2),
                              Point<double>(4, 1), 
                              Point<double>(3, 4), 
                              Point<double>(5, 3)
                              );

    EXPECT_DOUBLE_EQ(test_1.calculate_area(), 5);
}

TEST(rotated_rectangle_tests, get_length_test) {
    Rectangle<double> test_1 (
                              Point<double>(2, 2), 
                              Point<double>(4, 1),
                              Point<double>(3, 4), 
                              Point<double>(5, 3)
                              );

    EXPECT_DOUBLE_EQ(test_1.get_length(), sqrt(5));
}

TEST(rotated_rectangle_tests, get_width_test) {
    Rectangle<double> test_1 (
                              Point<double>(2, 2), 
                              Point<double>(4, 1), 
                              Point<double>(3, 4), 
                              Point<double>(5, 3)
                              );

    EXPECT_DOUBLE_EQ(test_1.get_width(), sqrt(5));
}

TEST(rotated_rectangle_tests, operator_double_test) {
    Rectangle<double> test_1 (
                              Point<double>(2, 2), 
                              Point<double>(4, 1), 
                              Point<double>(3, 4), 
                              Point<double>(5, 3)
                              );
    double operator_double_test_val = double(test_1);

    EXPECT_DOUBLE_EQ(operator_double_test_val, 5);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}