#pragma once
#include "Money.hpp"

namespace casino::domain{
enum class BetType { Color, Number };
class Bet final{
private:
    const BetType type_;
    const Money amount_;
public:
    explicit Bet(BetType type, Money amount);
    Bet(const Bet&) = delete;
    Bet& operator=(const Bet&) = delete;
    Bet(Bet&&) = delete;
    Bet& operator=(Bet&&) = delete;

    ~Bet() = default;

    [[nodiscard]] BetType getType() const noexcept { return type_; }
    [[nodiscard]] Money getAmount() const noexcept { return amount_; }
};
}