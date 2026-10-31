#include <gtest/gtest.h>

#include "infrastructure/Mt19937Generator.hpp"

using namespace casino;

TEST(Mt19937GeneratorTest, SameSeedSameNumbers) {
    Mt19937Generator a(42);
    Mt19937Generator b(42);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(a.nextInt(0, 36), b.nextInt(0, 36));
    }
}

TEST(Mt19937GeneratorTest, NumbersInRange) {
    Mt19937Generator generator(7);
    for (int i = 0; i < 1000; ++i) {
        int number = generator.nextInt(0, 36);
        EXPECT_GE(number, 0);
        EXPECT_LE(number, 36);
    }
    EXPECT_THROW(generator.nextInt(10, 1), std::invalid_argument);
}
