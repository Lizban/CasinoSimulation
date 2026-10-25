#include "domain/Bet.hpp"

#include <stdexcept>

namespace casino {

std::string toString(BetType type) {
    return type == BetType::Color ? "color" : "number";
}

Bet::Bet(BetType type, Money amount, Color color, int number)
    : type_(type), amount_(amount), color_(color), number_(number) {
    if (amount.chips() == 0) {
        throw std::invalid_argument("Ставка должна быть больше нуля");
    }
}

Bet Bet::onColor(Color color, Money amount) {
    if (color == Color::Green) {
        throw std::invalid_argument("На цвет можно ставить только красное или чёрное");
    }
    return Bet(BetType::Color, amount, color, 0);
}

Bet Bet::onNumber(int number, Money amount) {
    if (!Pocket::isValidNumber(number)) {
        throw std::invalid_argument("Ставить можно на число от 0 до 36");
    }
    return Bet(BetType::Number, amount, Color::Green, number);
}

bool Bet::winsOn(const Pocket& pocket) const {
    if (type_ == BetType::Color) {
        return pocket.color() == color_;
    }
    return pocket.number() == number_;
}

int Bet::payoutRatio() const {
    return type_ == BetType::Color ? 1 : 35;
}

Money Bet::winnings() const {
    return amount_ * payoutRatio();
}

std::string Bet::targetName() const {
    return type_ == BetType::Color ? toString(color_) : std::to_string(number_);
}

}
