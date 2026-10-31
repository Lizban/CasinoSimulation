#include <gtest/gtest.h>

#include "FakeGenerator.hpp"
#include "application/BotRunner.hpp"

using namespace casino;

TEST(BotRunnerTest, SpinLimit) {
    FakeGenerator generator({1});
    RouletteService service(Player("Бот", Money(100)), Cashbox(Money(1000)),
                            TableLimits(Money(1), Money(50)), generator);
    FlatStrategy strategy(Money(10), Color::Red);
    BotReport report = runBot(service, strategy, StopRules{5, Money(), Money()});
    EXPECT_EQ(report.spins, 5);
    EXPECT_EQ(report.reason, BotStopReason::SpinLimit);
    EXPECT_EQ(report.finalBalance, Money(150));
    EXPECT_FALSE(toString(report.reason).empty());
}

TEST(BotRunnerTest, WinGoal) {
    FakeGenerator generator({1});
    RouletteService service(Player("Бот", Money(100)), Cashbox(Money(1000)),
                            TableLimits(Money(1), Money(50)), generator);
    FlatStrategy strategy(Money(10), Color::Red);
    BotReport report = runBot(service, strategy, StopRules{100, Money(), Money(30)});
    EXPECT_EQ(report.reason, BotStopReason::WinGoal);
    EXPECT_EQ(report.finalBalance, Money(130));
}

TEST(BotRunnerTest, LossLimit) {
    FakeGenerator generator({2});
    RouletteService service(Player("Бот", Money(100)), Cashbox(Money(1000)),
                            TableLimits(Money(1), Money(50)), generator);
    FlatStrategy strategy(Money(10), Color::Red);
    BotReport report = runBot(service, strategy, StopRules{100, Money(30), Money()});
    EXPECT_EQ(report.reason, BotStopReason::LossLimit);
    EXPECT_EQ(report.finalBalance, Money(70));
}
