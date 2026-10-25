#include <gtest/gtest.h>

#include "FakeGenerator.hpp"
#include "domain/RouletteWheel.hpp"

using namespace casino;

TEST(RouletteWheelTest, Spin) {
    FakeGenerator generator({17});
    RouletteWheel wheel(generator);
    EXPECT_EQ(wheel.spin().number(), 17);
}
