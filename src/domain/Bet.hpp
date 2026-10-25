#pragma once

#include <string>

#include "domain/Money.hpp"
#include "domain/Pocket.hpp"

namespace casino {

enum class BetType { Color, Number };

std::string toString(BetType type);

class Bet {
public:
    static Bet onColor(Color color, Money amount);
    static Bet onNumber(int number, Money amount);

    BetType type() const { return type_; }
    Money amount() const { return amount_; }
    Color color() const { return color_; }
    int number() const { return number_; }

    bool winsOn(const Pocket& pocket) const;

    int payoutRatio() const;

    Money winnings() const;

    std::string targetName() const;

private:
    Bet(BetType type, Money amount, Color color, int number);

    BetType type_;
    Money amount_;
    Color color_;
    int number_;
};

}
