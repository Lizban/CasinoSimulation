#include <gtest/gtest.h>

#include "domain/TableLimits.hpp"

using namespace casino;

TEST(TableLimitsTest, Allows) {
    TableLimits limits(Money(5), Money(100));
    EXPECT_TRUE(limits.allows(Money(5)));
    EXPECT_TRUE(limits.allows(Money(100)));
    EXPECT_FALSE(limits.allows(Money(4)));
    EXPECT_FALSE(limits.allows(Money(101)));
}

TEST(TableLimitsTest, WrongLimits) {
    EXPECT_THROW(TableLimits(Money(0), Money(10)), std::invalid_argument);
    EXPECT_THROW(TableLimits(Money(20), Money(10)), std::invalid_argument);
}
