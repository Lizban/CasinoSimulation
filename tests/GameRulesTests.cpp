#include <gtest/gtest.h>

#include "domain/GameRules.hpp"

using namespace casino;

TEST(GameRulesTest, GameOver) {
    TableLimits limits(Money(5), Money(50));
    EXPECT_EQ(checkGameOver(Player("Анна", Money(100)), Cashbox(Money(100)), limits),
              GameOverReason::None);
    EXPECT_EQ(checkGameOver(Player("Анна", Money(4)), Cashbox(Money(100)), limits),
              GameOverReason::PlayerCannotBet);
    EXPECT_EQ(checkGameOver(Player("Анна", Money(100)), Cashbox(Money(4)), limits),
              GameOverReason::CasinoCannotPay);
    EXPECT_FALSE(toString(GameOverReason::PlayerCannotBet).empty());
}
