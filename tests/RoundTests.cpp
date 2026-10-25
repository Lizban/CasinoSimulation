#include <gtest/gtest.h>

#include "domain/Round.hpp"

using namespace casino;

TEST(RoundTest, BetIsTakenFromPlayer) {
    Player player("Анна", Money(100));
    Cashbox cashbox(Money(1000));
    Round round(player, cashbox, TableLimits(Money(1), Money(50)));
    round.place(Bet::onColor(Color::Red, Money(10)));
    EXPECT_EQ(player.balance(), Money(90));
    EXPECT_EQ(round.bets().size(), 1);
}

TEST(RoundTest, WrongBetsAreRejected) {
    Player player("Анна", Money(20));
    Cashbox cashbox(Money(100));
    Round round(player, cashbox, TableLimits(Money(5), Money(50)));
    EXPECT_EQ(round.check(Bet::onColor(Color::Red, Money(1))), BetCheck::BelowTableMin);
    EXPECT_EQ(round.check(Bet::onColor(Color::Red, Money(60))), BetCheck::AboveTableMax);
    EXPECT_EQ(round.check(Bet::onColor(Color::Red, Money(30))), BetCheck::NotEnoughPlayerMoney);
    EXPECT_EQ(round.check(Bet::onNumber(7, Money(10))), BetCheck::CasinoCannotCover);
    EXPECT_THROW(round.place(Bet::onColor(Color::Red, Money(60))), BetRejected);
}

TEST(RoundTest, ColorWin) {
    Player player("Анна", Money(100));
    Cashbox cashbox(Money(1000));
    Round round(player, cashbox, TableLimits(Money(1), Money(50)));
    round.place(Bet::onColor(Color::Red, Money(10)));
    RoundResult result = round.settle(Pocket(1));
    EXPECT_TRUE(result.outcomes[0].won);
    EXPECT_EQ(player.balance(), Money(110));
    EXPECT_EQ(cashbox.balance(), Money(990));
}

TEST(RoundTest, NumberWin) {
    Player player("Анна", Money(100));
    Cashbox cashbox(Money(1000));
    Round round(player, cashbox, TableLimits(Money(1), Money(50)));
    round.place(Bet::onNumber(7, Money(10)));
    round.settle(Pocket(7));
    EXPECT_EQ(player.balance(), Money(450));
    EXPECT_EQ(cashbox.balance(), Money(650));
}

TEST(RoundTest, LoseGoesToCashbox) {
    Player player("Анна", Money(100));
    Cashbox cashbox(Money(1000));
    Round round(player, cashbox, TableLimits(Money(1), Money(50)));
    round.place(Bet::onColor(Color::Red, Money(10)));
    round.settle(Pocket(0));
    EXPECT_EQ(player.balance(), Money(90));
    EXPECT_EQ(cashbox.balance(), Money(1010));
}

TEST(RoundTest, SettleOnlyOnce) {
    Player player("Анна", Money(100));
    Cashbox cashbox(Money(1000));
    Round round(player, cashbox, TableLimits(Money(1), Money(50)));
    EXPECT_THROW(round.settle(Pocket(1)), std::logic_error);
    round.place(Bet::onColor(Color::Red, Money(10)));
    round.settle(Pocket(1));
    EXPECT_THROW(round.settle(Pocket(1)), std::logic_error);
    EXPECT_THROW(round.place(Bet::onColor(Color::Red, Money(10))), std::logic_error);
}
