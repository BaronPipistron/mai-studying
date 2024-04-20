#include <gtest/gtest.h>
#include "../include/figures/Square.h"
#include "../src/figures/Square.cpp"
#include "../include/figures/Point.h"
#include "../src/figures/Point.cpp"

using namespace figures;

TEST(constructor_test, without_exception_test_1) {
    ASSERT_NO_THROW(
                    Square<double>(
                                   Point<double>(0, 0), 
                                   Point<double>(1, 0), 
                                   Point<double>(0, 1), 
                                   Point<double>(1, 1)
                                   )
                    );
}

TEST(constructor_test, without_exception_test_2) {
    ASSERT_NO_THROW(
                    Square<double>(
                                   Point<double>(1.5, 1.5), 
                                   Point<double>(5.5, 1.5), 
                                   Point<double>(1.5, 5.5), 
                                   Point<double>(5.5, 5.5)
                                   )
                    );
}

TEST(constructor_test, with_exception_test_1) {
    ASSERT_THROW(
                 Square<double>(
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
                 Square<double>(
                                Point<double>(3.4, 43), 
                                Point<double>(2, 78), 
                                Point<double>(-31, 5), 
                                Point<double>(34, 49)
                                ), 
                 std::invalid_argument
                 );
}

TEST(side_constructor_test, without_exception_test_1) {
    ASSERT_NO_THROW(Square<double>(5));
}

TEST(side_constructor_test, without_exception_test_2) {
    ASSERT_NO_THROW(Square<double>(4.65));
}

TEST(side_constructor_test, with_exception_test_1) {
    ASSERT_THROW(
                 Square<double>(0), 
                 std::invalid_argument
                 );
}

TEST(side_constructor_test, with_exception_test_2) {
    ASSERT_THROW(
                 Square<double>(-43.685), 
                 std::invalid_argument
                 );
}

TEST(copy_constructor_test, other_square_copy_test) {
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(1, 0), 
                           Point<double>(0, 1), 
                           Point<double>(1, 1)
                           );
    Square<double> test_2 (test_1);

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_side(), test_2.get_side());
}

TEST(move_constructor_test, other_square_move_test) {
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(1, 0), 
                           Point<double>(0, 1), 
                           Point<double>(1, 1)
                           );
    Square<double> test_2 (std::move(test_1));

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_side(), test_2.get_side());
}

TEST(assigment_operator_test, copy_assigment_test) {
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(1, 0), 
                           Point<double>(0, 1), 
                           Point<double>(1, 1)
                           );
    Square<double> test_2 = test_1;

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_side(), test_2.get_side());
}

TEST(assigment_operator_test, move_assigment_test) {
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(1, 0), 
                           Point<double>(0, 1), 
                           Point<double>(1, 1)
                           );
    Square<double> test_2 = std::move(test_1);

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_side(), test_2.get_side());
}

TEST(calculate_geometric_center_test, calculate_geometric_center_test_1) {
    Square<double> test_1 (
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
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(9, 0), 
                           Point<double>(0, 9), 
                           Point<double>(9, 9)
                           );
    Point<double> center_test = test_1.calculate_geometric_center();

    EXPECT_DOUBLE_EQ(center_test.get_x_cord(), 4.5);
    EXPECT_DOUBLE_EQ(center_test.get_y_cord(), 4.5);
}

TEST(calculate_perimeter_test, calculate_perimeter_test_1) {
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(2, 0), 
                           Point<double>(0, 2), 
                           Point<double>(2, 2)
                           );

    EXPECT_DOUBLE_EQ(test_1.calculate_perimeter(), 8);
}

TEST(calculate_perimeter_test, calculate_perimeter_test_2) {
    Square<double> test_1 (5.65);

    EXPECT_DOUBLE_EQ(test_1.calculate_perimeter(), 22.6);
}

TEST(calculate_area_test, calculate_area_test_1) {
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(2, 0), 
                           Point<double>(0, 2), 
                           Point<double>(2, 2)
                           );

    EXPECT_DOUBLE_EQ(test_1.calculate_area(), 4);
}

TEST(calculate_area_test, calculate_area_test_2) {
    Square<double> test_1 (5.65);

    EXPECT_DOUBLE_EQ(test_1.calculate_area(), 31.9225);
}

TEST(get_side_test, get_side_test_1) {
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(2, 0), 
                           Point<double>(0, 2), 
                           Point<double>(2, 2)
                           );

    EXPECT_DOUBLE_EQ(test_1.get_side(), 2);
}

TEST(get_side_test, get_side_test_2) {
    Square<double> test_1 (5.678);

    EXPECT_DOUBLE_EQ(test_1.get_side(), 5.678);
}

TEST(operator_double_test, operator_double_test_1) {
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(2, 0), 
                           Point<double>(0, 2), 
                           Point<double>(2, 2)
                           );
    double operator_double_test_val = double(test_1);

    EXPECT_DOUBLE_EQ(operator_double_test_val, 4);
}

TEST(operator_double_test, operator_double_test_2) {
    Square<double> test_1 (5.65);
    double operator_double_test_val = double(test_1);

    EXPECT_DOUBLE_EQ(operator_double_test_val, 31.9225);
}

TEST(equal_operator_test, true_return_1) {
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(2, 0), 
                           Point<double>(0, 2), 
                           Point<double>(2, 2)
                           );

    Square<double> test_2 (
                           Point<double>(0, 0), 
                           Point<double>(2, 0), 
                           Point<double>(0, 2), 
                           Point<double>(2, 2)
                           );

    EXPECT_TRUE(test_1 == test_2);
}

TEST(equal_operator_test, true_return_2) {
    Square<double> test_1 (5.68);
    Square<double> test_2 (5.68);

    EXPECT_TRUE(test_1 == test_2);
}

TEST(equal_operator_test, false_return_1) {
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(2, 0), 
                           Point<double>(0, 2), 
                           Point<double>(2, 2)
                           );

    Square<double> test_2 (
                           Point<double>(0, 0), 
                           Point<double>(4, 0), 
                           Point<double>(0, 4), 
                           Point<double>(4, 4)
                           );

    EXPECT_FALSE(test_1 == test_2);
}

TEST(equal_operator_test, false_return_2) {
    Square<double> test_1 (6.568);
    Square<double> test_2 (7.4254);

    EXPECT_FALSE(test_1 == test_2);
}

TEST(non_equal_operator_test, true_return_1) {
    Square<double> test_1 (
                           Point<double>(0, 0),
                           Point<double>(2, 0), 
                           Point<double>(0, 2), 
                           Point<double>(2, 2)
                           );

    Square<double> test_2 (
                           Point<double>(0, 0), 
                           Point<double>(4, 0), 
                           Point<double>(0, 4), 
                           Point<double>(4, 4)
                           );

    EXPECT_TRUE(test_1 != test_2);
}

TEST(non_equal_operator_test, true_return_2) {
    Square<double> test_1 (6.568);
    Square<double> test_2 (7.4254);

    EXPECT_TRUE(test_1 != test_2);
}

TEST(non_equal_operator_test, false_return_1) {
    Square<double> test_1 (
                           Point<double>(0, 0), 
                           Point<double>(2, 0), 
                           Point<double>(0, 2), 
                           Point<double>(2, 2)
                           );

    Square<double> test_2 (
                           Point<double>(0, 0), 
                           Point<double>(2, 0), 
                           Point<double>(0, 2), 
                           Point<double>(2, 2)
                           );

    EXPECT_FALSE(test_1 != test_2);
}

TEST(non_equal_operator_test, false_return_2) {
    Square<double> test_1 (5.68);
    Square<double> test_2 (5.68);

    EXPECT_FALSE(test_1 != test_2);
}

TEST(rotated_square_tests, constructor_test_without_exception) {
    ASSERT_NO_THROW(
                    Square<double>(
                                   Point<double>(2, 2), 
                                   Point<double>(4, 1), 
                                   Point<double>(3, 4), 
                                   Point<double>(5, 3)
                                   )
                    );
}

TEST(rotated_square_tests, copy_constructor_test) {
    Square<double> test_1 (
                           Point<double>(2, 2), 
                           Point<double>(4, 1), 
                           Point<double>(3, 4), 
                           Point<double>(5, 3)
                           );
    Square<double> test_2 (test_1);

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_side(), test_2.get_side());
}

TEST(rotated_square_tests, move_constructor_test) {
    Square<double> test_1 (
                           Point<double>(2, 2), 
                           Point<double>(4, 1), 
                           Point<double>(3, 4), 
                           Point<double>(5, 3)
                           );
    Square<double> test_2 (std::move(test_1));

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_side(), test_2.get_side());
}

TEST(rotated_square_tests, assigment_operator_copy_test) {
    Square<double> test_1 (
                           Point<double>(2, 2), 
                           Point<double>(4, 1), 
                           Point<double>(3, 4), 
                           Point<double>(5, 3)
                           );
    Square<double> test_2 = test_1;

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_side(), test_2.get_side());
}

TEST(rotated_square_tests, assigment_operator_move_test) {
    Square<double> test_1 (
                           Point<double>(2, 2), 
                           Point<double>(4, 1), 
                           Point<double>(3, 4), 
                           Point<double>(5, 3)
                           );
    Square<double> test_2 = std::move(test_1);

    EXPECT_TRUE(test_1 == test_2);
    EXPECT_DOUBLE_EQ(test_1.get_side(), test_2.get_side());
}

TEST(rotated_square_tests, calculate_geometric_center_test) {
    Square<double> test_1 (
                           Point<double>(2, 2), 
                           Point<double>(4, 1), 
                           Point<double>(3, 4), 
                           Point<double>(5, 3)
                           );
    Point<double> center_test = test_1.calculate_geometric_center();

    EXPECT_DOUBLE_EQ(center_test.get_x_cord(), 3.5);
    EXPECT_DOUBLE_EQ(center_test.get_y_cord(), 2.5);
}

TEST(rotated_square_tests, calculate_perimeter_test) {
    Square<double> test_1 (
                           Point<double>(2, 2), 
                           Point<double>(4, 1), 
                           Point<double>(3, 4), 
                           Point<double>(5, 3)
                           );

    EXPECT_DOUBLE_EQ(test_1.calculate_perimeter(), sqrt(5) * 4);
}

TEST(rotated_square_tests, calculate_area_test) {
    Square<double> test_1 (
                           Point<double>(2, 2), 
                           Point<double>(4, 1), 
                           Point<double>(3, 4), 
                           Point<double>(5, 3)
                           );

    EXPECT_DOUBLE_EQ(test_1.calculate_area(), 5);
}

TEST(rotated_square_tests, get_side_test) {
    Square<double> test_1 (
                           Point<double>(2, 2), 
                           Point<double>(4, 1), 
                           Point<double>(3, 4), 
                           Point<double>(5, 3)
                           );

    EXPECT_DOUBLE_EQ(test_1.get_side(), sqrt(5));
}

TEST(rotated_square_tests, operator_double_test) {
    Square<double> test_1 (
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