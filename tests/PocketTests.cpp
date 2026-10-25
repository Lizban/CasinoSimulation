#include <gtest/gtest.h>

#include "domain/Pocket.hpp"

using namespace casino;

TEST(PocketTest, Colors) {
    EXPECT_EQ(Pocket(0).color(), Color::Green);
    EXPECT_EQ(Pocket(1).color(), Color::Red);
    EXPECT_EQ(Pocket(2).color(), Color::Black);
    EXPECT_EQ(Pocket(11).color(), Color::Black);
    EXPECT_EQ(Pocket(12).color(), Color::Red);
}

TEST(PocketTest, NumberMustBeFrom0To36) {
    EXPECT_EQ(Pocket(36).number(), 36);
    EXPECT_THROW(Pocket(37), std::invalid_argument);
    EXPECT_THROW(Pocket(-1), std::invalid_argument);
}

TEST(PocketTest, ColorToString) {
    EXPECT_EQ(toString(Color::Red), "red");
    EXPECT_EQ(toString(Color::Black), "black");
    EXPECT_EQ(toString(Color::Green), "green");
}
