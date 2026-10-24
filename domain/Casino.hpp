#pragma once
#include "Money.hpp"
#include "Bet.hpp"

namespace casino::domain{
class Casino final{
private:
    Money balance;
    Money table_min;
    Money table_max;
public:
    explicit Casino(Money balance, Money table_min, Money table_max);
    Casino(const Casino&) = delete;
    Casino& operator=(const Casino&) = delete;
    Casino(Casino&&) noexcept = default;
    Casino& operator=(Casino&&) noexcept = default;

    ~Casino() = default;

    [[nodiscard]] bool canAcceptBet(const Bet& bet) const noexcept;

    void takeMoney(Money amount);
    void payOut(Money amount);
};
}