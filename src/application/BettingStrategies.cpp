#include "application/BettingStrategies.hpp"

#include <algorithm>
#include <stdexcept>

namespace casino {

namespace {

Money fitToTable(Money wanted, Money balance, const TableLimits& limits) {
    Money amount = std::min({wanted, limits.max(), balance});
    return std::max(amount, limits.min());
}

bool anyBetWon(const RoundResult& result) {
    return std::any_of(result.outcomes.begin(), result.outcomes.end(),
                       [](const BetOutcome& outcome) { return outcome.won; });
}

}

FlatStrategy::FlatStrategy(Money amount, Color color) : amount_(amount), color_(color) {}

Bet FlatStrategy::nextBet(Money balance, const TableLimits& limits) {
    return Bet::onColor(color_, fitToTable(amount_, balance, limits));
}

void FlatStrategy::onResult(const RoundResult&) {
}

MartingaleStrategy::MartingaleStrategy(Money baseAmount, Color color)
    : baseAmount_(baseAmount), lastAmount_(baseAmount), nextAmount_(baseAmount), color_(color) {}

Bet MartingaleStrategy::nextBet(Money balance, const TableLimits& limits) {
    lastAmount_ = fitToTable(nextAmount_, balance, limits);
    return Bet::onColor(color_, lastAmount_);
}

void MartingaleStrategy::onResult(const RoundResult& result) {
    nextAmount_ = anyBetWon(result) ? baseAmount_ : lastAmount_ * 2;
}

std::unique_ptr<IBettingStrategy> makeStrategy(const std::string& name, Money baseAmount) {
    if (name == "flat") {
        return std::make_unique<FlatStrategy>(baseAmount, Color::Red);
    }
    if (name == "martingale") {
        return std::make_unique<MartingaleStrategy>(baseAmount, Color::Red);
    }
    throw std::invalid_argument("Неизвестная стратегия: " + name + " (есть flat и martingale)");
}

}
