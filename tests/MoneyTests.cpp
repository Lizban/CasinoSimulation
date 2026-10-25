#include <gtest/gtest.h>

#include "domain/Money.hpp"

using namespace casino;

TEST(MoneyTest, Create) {
    EXPECT_EQ(Money().chips(), 0);
    EXPECT_EQ(Money(150).chips(), 150);
    EXPECT_THROW(Money(-1), std::invalid_argument);
}

TEST(MoneyTest, AddAndSubtract) {
    EXPECT_EQ(Money(10) + Money(5), Money(15));
    EXPECT_EQ(Money(10) - Money(4), Money(6));
    EXPECT_THROW(Money(5) - Money(6), std::invalid_argument);
}

TEST(MoneyTest, Multiply) {
    EXPECT_EQ(Money(10) * 35, Money(350));
    EXPECT_THROW(Money(10) * -1, std::invalid_argument);
}

TEST(MoneyTest, Compare) {
    EXPECT_TRUE(Money(1) < Money(2));
    EXPECT_TRUE(Money(3) >= Money(3));
    EXPECT_TRUE(Money(1) != Money(2));
}
