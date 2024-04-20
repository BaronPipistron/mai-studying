#include <gtest/gtest.h>
#include "../include/vector.hpp"
#include "../include/Twelve.hpp"

using namespace MAI::OOP::Lab2;

TEST(constructor_test, default_constructor_test) {
    vector<Twelve> vec; 

    EXPECT_EQ(vec.capacity(), 1);
    EXPECT_EQ(vec.size(), 0);
}

TEST(constructor_test, initializer_constructor_test) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    Twelve d { "BBA" };
    Twelve e { "0" };
    Twelve f { "BA14" };
    vector<Twelve> vec{a, b, c, d, e, f};

    EXPECT_EQ(vec.capacity(), 6);
    EXPECT_EQ(vec.size(), 6);
}

TEST(assignment_operator_test, copy_assignment) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    Twelve d { "BBA" };

    vector<Twelve> vec{a, b, c, d};
    vector<Twelve> vec1;

    vec1 = vec;

    EXPECT_EQ(vec1.size(), 4);
    EXPECT_EQ(vec1.capacity(), 4);

    for (size_t i = 0; i != vec.size(); ++i) {
        EXPECT_EQ(vec1[i], vec[i]);
    }

    EXPECT_TRUE(vec == vec1);
}

TEST(size_test, empty_vector) {
    vector<Twelve> v;
    size_t size = v.size();

    EXPECT_EQ(size, 0);
}

TEST(size_test, non_empty_vector) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    Twelve d { "BBA" };
    Twelve e { "0" };
    Twelve f { "BA14" };
    vector<Twelve> v{a, b, c, d, e, f};
    size_t size = v.size();

    EXPECT_EQ(size, 6);
}

TEST(capacity_test, empty_vector) {
    vector<Twelve> v;
    size_t capacity = v.capacity(); 

    EXPECT_EQ(capacity, 1);
}

TEST(capacity_test, non_empty_vector) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    Twelve d { "BBA" };
    Twelve e { "0" };
    Twelve f { "BA14" };
    vector<Twelve> v{a, b, c, d, e, f};
    size_t capacity = v.capacity(); 

    EXPECT_EQ(capacity, 6);
}

TEST(empty_test, empty_vector) {
    vector<Twelve> v;
    bool is_empty = v.empty(); 

    ASSERT_TRUE(is_empty);
}

TEST(empty_test, non_empty_vector) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    vector<Twelve> v{a, b, c};
    bool is_empty = v.empty(); 
  
    ASSERT_FALSE(is_empty);
}

TEST(empty_test, const_empty_vector) {
    vector<Twelve> v;
    bool is_empty = v.empty(); 

    ASSERT_TRUE(is_empty);
}

TEST(empty_test, const_non_empty_vector) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    vector<Twelve> v{a, b, c};
    bool is_empty = v.empty(); 
  
    ASSERT_FALSE(is_empty);
}

TEST(equal_operator_test, equal_vectors) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    vector<Twelve> v{a, b, c};
    vector<Twelve> v1 = v;
  
    bool is_equal = (v1 == v);
    EXPECT_TRUE(is_equal);
}

TEST(equal_operator_test, non_equal_vectors) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    Twelve d { "BBA" };
    Twelve e { "0" };
    Twelve f { "BA14" };
    vector<Twelve> v{a, b, c};
    vector<Twelve> v1{a, b, c, d, e, f};
  
    bool is_equal = (v1 == v);
    EXPECT_FALSE(is_equal);
}

TEST(non_equal_operator_test, equal_vectors) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    vector<Twelve> v{a, b, c};
    vector<Twelve> v1 = v;
  
    bool is_equal = (v1 != v);
    EXPECT_FALSE(is_equal);
}

TEST(non_equal_operator_test, non_equal_vectors) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    Twelve d { "BBA" };
    Twelve e { "0" };
    Twelve f { "BA14" };
    vector<Twelve> v{a, b, c};
    vector<Twelve> v1{a, b, c, d, e, f};
  
    bool is_equal = (v1 != v);
    EXPECT_TRUE(is_equal);
}

TEST(resize_test, resize_vector) {
    vector<Twelve> v;
    size_t size = 10;
    v.resize(size, Twelve{ "1" }); 

    EXPECT_EQ(v.capacity(), size);
    EXPECT_EQ(v.size(), size);

    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], Twelve{ "1" });
    }
}

TEST(reserve_test, empty_vector) {
    vector<Twelve> v;
    size_t capacity = 100;

    v.reserve(capacity); 

    EXPECT_EQ(v.capacity(), capacity);
}

TEST(pop_back_test, non_empty_vector) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    vector<Twelve> v{a, b, c};
    size_t size = v.size();

    v.pop_back(); // TESTED

    EXPECT_EQ(size - 1, v.size());
    EXPECT_EQ(v[v.size() - 1], Twelve{ "AB" });
}

TEST(push_back_test, non_empty_vector) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    vector<Twelve> v{123, 145, 12345};
    size_t size = v.size();

    v.push_back(Twelve{ "12345" }); // TESTED

    EXPECT_EQ(v.size(), size + 1);
    EXPECT_EQ(v[v.size() - 1], Twelve{ "12345" });
}

TEST(get_element_operator_test, non_empty_vector) {
    Twelve a { "14" };
    Twelve b { "AB" };
    Twelve c { "41" };
    Twelve d { "BBA" };
    vector<Twelve> v{a, b, c, d};

    Twelve el0 = v[0]; // TESTED
    Twelve el1 = v[1]; // 
    Twelve el2 = v[2]; //  
    Twelve el3 = v[3]; //

    EXPECT_EQ(el0, Twelve{ "14" });
    EXPECT_EQ(el1, Twelve{ "AB" });
    EXPECT_EQ(el2, Twelve{ "41" });
    EXPECT_EQ(el3, Twelve{ "BBA" });
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}