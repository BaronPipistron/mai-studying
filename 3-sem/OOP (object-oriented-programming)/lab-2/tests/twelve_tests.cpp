#include <gtest/gtest.h>
#include "../include/Twelve.hpp"
#include "../src/Twelve.cpp"

using namespace MAI::OOP::Lab2;

TEST(constructor_test, without_exception_test) {
    ASSERT_NO_THROW(Twelve(10, '5'));
}

TEST(initializer_list_constructor_test, exception_test) {
    ASSERT_THROW(Twelve { 'H' }, std::invalid_argument);
}

TEST(initializer_list_constructor_test, without_exception_test) {
    ASSERT_NO_THROW(Twelve { 'B' });
}

TEST(string_constructor_test, exception_test) {
    ASSERT_THROW(Twelve { "1H93J" }, std::invalid_argument);
}

TEST(string_constructor_test, without_exception_test_0) {
    ASSERT_NO_THROW(Twelve { "0" });
}

TEST(string_constructor_test, without_exception_test_1) {
    ASSERT_NO_THROW(Twelve{ "51B35" });
}

TEST(to_DEC_test, test1) {
    Twelve test { "AAB3" };
    uint64_t dec_num = test.to_DEC();
    testing::internal::CaptureStdout();
    std::cout << dec_num << std::endl;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "18855\n");
}

TEST(to_DEC_test, test2) {
    Twelve test { "14B" };
    uint64_t dec_num = test.to_DEC();
    testing::internal::CaptureStdout();
    std::cout << dec_num << std::endl;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "203\n");
}

TEST(to_BIN_test, test1) {
    Twelve test { "AAB3" };
    std::string bin_num = test.to_BIN();
    testing::internal::CaptureStdout();
    std::cout << bin_num << std::endl;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "100100110100111\n");
}

TEST(to_BIN_test, test2) {
    Twelve test { "141BA41" };
    std::string bin_num = test.to_BIN();
    testing::internal::CaptureStdout();
    std::cout << bin_num << std::endl;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1111010110000100010001\n");
}

TEST(operator_plus_equal_test, test1) {
    Twelve test_a { "AAB3" };
    Twelve test_b { "143" };
    test_a += test_b;
    testing::internal::CaptureStdout();
    test_a.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "B036\n");
}

TEST(operator_plus_equal_test, test2) {
    Twelve test_a { "1" };
    Twelve test_b { "BBB" };
    test_a += test_b;
    testing::internal::CaptureStdout();
    test_a.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1000\n");
}

TEST(operator_minus_equal_test, test1) {
    Twelve test_a { "AAB3" };
    Twelve test_b { "143" };
    test_a -= test_b;
    testing::internal::CaptureStdout();
    test_a.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "A970\n");
}

TEST(operator_minus_equal_test, test2) {
    Twelve test_a { "BBB" };
    Twelve test_b { "1" };
    test_a -= test_b;
    testing::internal::CaptureStdout();
    test_a.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "BBA\n");
}

TEST(operator_pref_inc_test, test1) {
    Twelve test{ "AA" };
    ++test;
    testing::internal::CaptureStdout();
    test.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "AB\n");
}

TEST(operator_pref_inc_test, test2) {
    Twelve test{ "10" };
    ++test;
    testing::internal::CaptureStdout();
    test.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "11\n");
}

TEST(operator_post_inc_test, test1) {
    Twelve test{ "AA" };
    test++;
    testing::internal::CaptureStdout();
    test.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "AB\n");
}

TEST(operator_post_inc_test, test2) {
    Twelve test{ "10" };
    test++;
    testing::internal::CaptureStdout();
    test.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "11\n");
}

TEST(operator_pref_dec_test, test1) {
    Twelve test{ "AB14" };
    --test;
    testing::internal::CaptureStdout();
    test.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "AB13\n");
}

TEST(operator_pref_dec_test, test2) {
    Twelve test{ "100" };
    --test;
    testing::internal::CaptureStdout();
    test.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "BB\n");
}

TEST(operator_post_dec_test, test1) {
    Twelve test{ "AB14" };
    test--;
    testing::internal::CaptureStdout();
    test.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "AB13\n");
}

TEST(operator_post_dec_test, test2) {
    Twelve test{ "100" };
    test--;
    testing::internal::CaptureStdout();
    test.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "BB\n");
}

TEST(comparison_operator_test, zero_true_test) {
    Twelve first{ "0" };
    Twelve second{ "0" };

    ASSERT_TRUE(first == second);
}

TEST(comparison_operator_test, true_return_1) {
    Twelve first{ "1A" };
    Twelve second{ "1A" };

    ASSERT_TRUE(first == second);
}

TEST(comparison_operator_test, true_return_2) {
    Twelve first{ "AAB" };
    Twelve second{ "AAB" };

    ASSERT_TRUE(first == second);
}

TEST(comparison_operator_test, false_return_1) {
    Twelve first{ "41BA" };
    Twelve second{ "42BA" };

    ASSERT_FALSE(first == second);
}

TEST(comparison_operator_test, false_return_2) {
    Twelve first{ "413B" };
    Twelve second{ "1BA341" };

    ASSERT_FALSE(first == second);
}

TEST(false_comparison_operator_test, true_return) {
    Twelve first{ "0" };
    Twelve second{ "1" };

    ASSERT_TRUE(first != second);
}

TEST(false_comparison_operator_test, false_return) {
    Twelve first{ "1BABA" };
    Twelve second{ "1BABA" };

    ASSERT_FALSE(first != second);
}

TEST(more_operator_test, true_return) {
    Twelve first{ "AB3" };
    Twelve second{ "110" };

    EXPECT_TRUE(first > second);
}

TEST(more_operator_test, false_return) {
    Twelve first{ "B2A39" };
    Twelve second{ "BBBBBBBBAABA" };

    EXPECT_FALSE(first > second);
}

TEST(more_operator_test, equals_args) {
    Twelve first{ "AA" };
    Twelve second{ "AA" };

    EXPECT_FALSE(first > second);
}

TEST(less_operator_test, true_return) {
    Twelve first{ "1A3" };
    Twelve second{ "AAABB" };

    EXPECT_TRUE(first < second);
}

TEST(less_operator_test, false_return) {
    Twelve first{ "4141B" };
    Twelve second{ "BA" };

    EXPECT_FALSE(first < second);
}

TEST(less_operator_test, equals_args) {
    Twelve first{ "BB" };
    Twelve second{ "BB" };

    EXPECT_FALSE(first < second);
}

TEST(more_or_equal_operator_test, true_return) {
    Twelve first{ "AB3" };
    Twelve second{ "110" };

    EXPECT_TRUE(first >= second);
}

TEST(more_or_equal_operator_test, false_return) {
    Twelve first{ "B2A39" };
    Twelve second{ "BBBBBBBBAABA" };

    EXPECT_FALSE(first >= second);
}

TEST(more_or_equal_operator_test, equals_args) {
    Twelve first{ "AA" };
    Twelve second{ "AA" };

    EXPECT_TRUE(first >= second);
}

TEST(less_or_equal_operator_test, true_return) {
    Twelve first{ "1A3" };
    Twelve second{ "AAABB" };

    EXPECT_TRUE(first <= second);
}

TEST(less_or_equal_operator_test, false_return) {
    Twelve first{ "4141B" };
    Twelve second{ "BA" };

    EXPECT_FALSE(first <= second);
}

TEST(less_or_equal_operator_test, equals_args) {
    Twelve first{ "BB" };
    Twelve second{ "BB" };

    EXPECT_TRUE(first <= second);
}

TEST(print_test, test1) {
    Twelve test{ "0" };
    testing::internal::CaptureStdout();
    test.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "0\n");
}

TEST(print_test, test2) {
    Twelve test{ "1AB56" };
    testing::internal::CaptureStdout();
    test.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1AB56\n");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}