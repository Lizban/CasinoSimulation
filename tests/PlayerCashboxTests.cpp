#include <gtest/gtest.h>

#include "domain/Cashbox.hpp"
#include "domain/Player.hpp"

using namespace casino;

TEST(PlayerTest, WithdrawAndDeposit) {
    Player player("Анна", Money(100));
    player.withdraw(Money(30));
    player.deposit(Money(5));
    EXPECT_EQ(player.balance(), Money(75));
    EXPECT_EQ(player.name(), "Анна");
}

TEST(PlayerTest, CannotWithdrawTooMuch) {
    Player player("Анна", Money(100));
    EXPECT_THROW(player.withdraw(Money(101)), std::logic_error);
    EXPECT_THROW(Player("", Money(100)), std::invalid_argument);
}

TEST(CashboxTest, ReceiveAndPay) {
    Cashbox cashbox(Money(1000));
    cashbox.receive(Money(10));
    cashbox.pay(Money(500));
    EXPECT_EQ(cashbox.balance(), Money(510));
    EXPECT_THROW(cashbox.pay(Money(600)), std::logic_error);
}
