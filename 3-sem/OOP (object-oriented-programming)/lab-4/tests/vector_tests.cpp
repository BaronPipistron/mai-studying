#include <gtest/gtest.h>
#include "../include/containers/vector.h"
#include "../src/containers/vector.cpp"
#include "../include/figures/Square.h"
#include "../src/figures/Square.cpp"
#include "../include/figures/Point.h"
#include "../src/figures/Point.cpp"

using namespace figures;
using square = Square<double>;

TEST(constructor_test, default_constructor_test) {
    vector<std::shared_ptr<Figure<double>>> test_1; 

    EXPECT_EQ(test_1.capacity(), 1);
    EXPECT_EQ(test_1.size(), 0);
}

TEST(constructor_test, initializer_constructor_test) {
    std::shared_ptr<square>  square_ptr1 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr2 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr3 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr4 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr5 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr6 = std::make_shared<square>(5);

    vector<std::shared_ptr<Figure<double>>> test_1 {
                                                    square_ptr1, 
                                                    square_ptr2, 
                                                    square_ptr3,
                                                    square_ptr4, 
                                                    square_ptr5, 
                                                    square_ptr6
                                                    };

    EXPECT_EQ(test_1.capacity(), 6);
    EXPECT_EQ(test_1.size(), 6);
}

TEST(constructor_test, copy_constructor_test) {
    std::shared_ptr<square>  square_ptr1 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr2 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr3 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr4 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr5 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr6 = std::make_shared<square>(5);

    vector<std::shared_ptr<Figure<double>>> test_1 {
                                                    square_ptr1, 
                                                    square_ptr2, 
                                                    square_ptr3,
                                                    square_ptr4, 
                                                    square_ptr5, 
                                                    square_ptr6
                                                    };

    vector<std::shared_ptr<Figure<double>>> test_2 (test_1);

    EXPECT_EQ(test_1.size(), 6);
    EXPECT_EQ(test_2.capacity(), 6);

    for (size_t i = 0; i != test_1.size(); ++i) {
        EXPECT_EQ(test_1[i], test_2[i]);
    }

    EXPECT_TRUE(test_1 == test_2);
}

TEST(assigment_operator_test, copy_assigment_test) {
    std::shared_ptr<square>  square_ptr1 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr2 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr3 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr4 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr5 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr6 = std::make_shared<square>(5);

    vector<std::shared_ptr<Figure<double>>> test_1 {
                                                    square_ptr1, 
                                                    square_ptr2, 
                                                    square_ptr3,
                                                    square_ptr4, 
                                                    square_ptr5, 
                                                    square_ptr6
                                                    };

    vector<std::shared_ptr<Figure<double>>> test_2 (test_1);

    EXPECT_EQ(test_1.size(), 6);
    EXPECT_EQ(test_2.capacity(), 6);

    for (size_t i = 0; i != test_1.size(); ++i) {
        EXPECT_EQ(test_1[i], test_2[i]);
    }

    EXPECT_TRUE(test_1 == test_2);
}

TEST(reserve_test, empty_vector) {
    vector<std::shared_ptr<Figure<double>>> test_1;
    size_t capacity = 100;

    test_1.reserve(capacity); 

    EXPECT_EQ(test_1.capacity(), capacity);
}

TEST(resize_test, resize_vector) {
    vector<std::shared_ptr<Figure<double>>> test_1;
    size_t size = 10;
    std::shared_ptr<square>  square_ptr1 = std::make_shared<square>(5);
    test_1.resize(size, square_ptr1); 

    EXPECT_EQ(test_1.capacity(), size);
    EXPECT_EQ(test_1.size(), size);

    for (size_t i = 0; i < test_1.size(); ++i) {
        EXPECT_EQ(test_1[i], square_ptr1);
    }
}

TEST(push_back_test, push_back_test) {
    std::shared_ptr<square>  square_ptr1 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr2 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr3 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr4 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr5 = std::make_shared<square>(5);

    vector<std::shared_ptr<Figure<double>>> test_1 {
                                                    square_ptr1, 
                                                    square_ptr2, 
                                                    square_ptr3,
                                                    square_ptr4, 
                                                    square_ptr5 
                                                    };

    size_t size = test_1.size();

    std::shared_ptr<square>  square_ptr6 = std::make_shared<square>(5);
    test_1.push_back(square_ptr6);

    EXPECT_EQ(test_1.size(), size + 1);
    EXPECT_EQ(test_1[test_1.size() - 1], square_ptr6);
}

TEST(pop_back_test, pop_back_test) {
    std::shared_ptr<square>  square_ptr1 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr2 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr3 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr4 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr5 = std::make_shared<square>(5);

    vector<std::shared_ptr<Figure<double>>> test_1 {
                                                    square_ptr1, 
                                                    square_ptr2, 
                                                    square_ptr3,
                                                    square_ptr4, 
                                                    square_ptr5
                                                    };

    size_t size = test_1.size();

    test_1.pop_back();

    EXPECT_EQ(test_1.size(), size - 1);
    EXPECT_EQ(test_1[test_1.size() - 1], square_ptr4);
}

TEST(erase_test, erase_test) {
    std::shared_ptr<square>  square_ptr1 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr2 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr3 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr4 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr5 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr6 = std::make_shared<square>(5);

    vector<std::shared_ptr<Figure<double>>> test_1 {
                                                    square_ptr1, 
                                                    square_ptr2, 
                                                    square_ptr3,
                                                    square_ptr4, 
                                                    square_ptr5
                                                    };

    size_t size = test_1.size();

    test_1.erase(2);
    EXPECT_EQ(test_1[2], square_ptr4);
    EXPECT_EQ(test_1.size(), size - 1);
}

TEST(size_test, size_test) {
    std::shared_ptr<square>  square_ptr1 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr2 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr3 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr4 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr5 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr6 = std::make_shared<square>(5);

    vector<std::shared_ptr<Figure<double>>> test_1 {
                                                    square_ptr1, 
                                                    square_ptr2, 
                                                    square_ptr3,
                                                    square_ptr4, 
                                                    square_ptr5, 
                                                    square_ptr6
                                                    };

    EXPECT_EQ(test_1.size(), 6);
}

TEST(capacity_test, capacity_test) {
    std::shared_ptr<square>  square_ptr1 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr2 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr3 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr4 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr5 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr6 = std::make_shared<square>(5);

    vector<std::shared_ptr<Figure<double>>> test_1 {
                                                    square_ptr1, 
                                                    square_ptr2, 
                                                    square_ptr3,
                                                    square_ptr4, 
                                                    square_ptr5, 
                                                    square_ptr6
                                                    };

    EXPECT_EQ(test_1.capacity(), 6);
}

TEST(emty_test, true_return) {
    vector<std::shared_ptr<Figure<double>>> test_1;

    EXPECT_TRUE(test_1.empty());
}

TEST(empty_test, false_return) {
    std::shared_ptr<square>  square_ptr1 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr2 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr3 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr4 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr5 = std::make_shared<square>(5);
    std::shared_ptr<square>  square_ptr6 = std::make_shared<square>(5);

    vector<std::shared_ptr<Figure<double>>> test_1 {
                                                    square_ptr1, 
                                                    square_ptr2, 
                                                    square_ptr3,
                                                    square_ptr4, 
                                                    square_ptr5, 
                                                    square_ptr6
                                                    };

    EXPECT_FALSE(test_1.empty());
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}