#pragma once
#include "Money.hpp"
#include "Bet.hpp"

namespace casino::domain{
class Player final{
private:
    Money balance;
    Money max_bet;
public:
    explicit Player(Money balance, Money max_bet);

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete; 
    Player(Player&&) noexcept = default;
    Player& operator=(Player&&) noexcept = default;

    ~Player() = default;

    [[nodiscard]] bool hasEnoughMoney(double amount) const noexcept;
    [[nodiscard]] bool canPlaceBet(const Bet& bet) const noexcept;

    void addToBalance(Money amount);
};
}