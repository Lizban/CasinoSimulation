#pragma once

#include <memory>
#include <string>

#include "domain/Bet.hpp"
#include "domain/Money.hpp"
#include "domain/Round.hpp"
#include "domain/TableLimits.hpp"

namespace casino {

class IBettingStrategy {
public:
    virtual ~IBettingStrategy() = default;

    virtual std::string name() const = 0;

    virtual Bet nextBet(Money balance, const TableLimits& limits) = 0;

    virtual void onResult(const RoundResult& result) = 0;
};

class FlatStrategy : public IBettingStrategy {
public:
    FlatStrategy(Money amount, Color color);

    std::string name() const override { return "flat"; }
    Bet nextBet(Money balance, const TableLimits& limits) override;
    void onResult(const RoundResult& result) override;

private:
    Money amount_;
    Color color_;
};

class MartingaleStrategy : public IBettingStrategy {
public:
    MartingaleStrategy(Money baseAmount, Color color);

    std::string name() const override { return "martingale"; }
    Bet nextBet(Money balance, const TableLimits& limits) override;
    void onResult(const RoundResult& result) override;

private:
    Money baseAmount_;
    Money lastAmount_;
    Money nextAmount_;
    Color color_;
};

std::unique_ptr<IBettingStrategy> makeStrategy(const std::string& name, Money baseAmount);

}
