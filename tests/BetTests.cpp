#include <gtest/gtest.h>

#include "domain/Bet.hpp"

using namespace casino;

TEST(BetTest, ColorBet) {
    Bet bet = Bet::onColor(Color::Red, Money(10));
    EXPECT_TRUE(bet.winsOn(Pocket(1)));
    EXPECT_FALSE(bet.winsOn(Pocket(2)));
    EXPECT_FALSE(bet.winsOn(Pocket(0)));
    EXPECT_EQ(bet.winnings(), Money(10));
    EXPECT_EQ(bet.targetName(), "red");
}

TEST(BetTest, NumberBet) {
    Bet bet = Bet::onNumber(17, Money(10));
    EXPECT_TRUE(bet.winsOn(Pocket(17)));
    EXPECT_FALSE(bet.winsOn(Pocket(18)));
    EXPECT_EQ(bet.winnings(), Money(350));
    EXPECT_EQ(bet.targetName(), "17");
}

TEST(BetTest, WrongBets) {
    EXPECT_THROW(Bet::onColor(Color::Green, Money(10)), std::invalid_argument);
    EXPECT_THROW(Bet::onNumber(40, Money(10)), std::invalid_argument);
    EXPECT_THROW(Bet::onColor(Color::Red, Money(0)), std::invalid_argument);
}
