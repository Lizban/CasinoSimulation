#include <gtest/gtest.h>

#include "application/BettingStrategies.hpp"

using namespace casino;

TEST(StrategyTest, Flat) {
    FlatStrategy strategy(Money(10), Color::Red);
    TableLimits limits(Money(1), Money(50));
    EXPECT_EQ(strategy.nextBet(Money(1000), limits).amount(), Money(10));
    EXPECT_EQ(strategy.nextBet(Money(5), limits).amount(), Money(5));
}

TEST(StrategyTest, Martingale) {
    MartingaleStrategy strategy(Money(10), Color::Red);
    TableLimits limits(Money(1), Money(50));
    Bet bet = Bet::onColor(Color::Red, Money(10));
    RoundResult lose{Pocket(2), {BetOutcome{bet, false, Money()}}};
    RoundResult win{Pocket(1), {BetOutcome{bet, true, Money(20)}}};

    EXPECT_EQ(strategy.nextBet(Money(1000), limits).amount(), Money(10));
    strategy.onResult(lose);
    EXPECT_EQ(strategy.nextBet(Money(1000), limits).amount(), Money(20));
    strategy.onResult(lose);
    EXPECT_EQ(strategy.nextBet(Money(1000), limits).amount(), Money(40));
    strategy.onResult(lose);
    EXPECT_EQ(strategy.nextBet(Money(1000), limits).amount(), Money(50));
    strategy.onResult(win);
    EXPECT_EQ(strategy.nextBet(Money(1000), limits).amount(), Money(10));
}

TEST(StrategyTest, MakeByName) {
    EXPECT_EQ(makeStrategy("flat", Money(10))->name(), "flat");
    EXPECT_EQ(makeStrategy("martingale", Money(10))->name(), "martingale");
    EXPECT_THROW(makeStrategy("abc", Money(10)), std::invalid_argument);
}
