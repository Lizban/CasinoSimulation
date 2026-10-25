#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "domain/Bet.hpp"
#include "domain/Cashbox.hpp"
#include "domain/Player.hpp"
#include "domain/Pocket.hpp"
#include "domain/TableLimits.hpp"

namespace casino {

enum class BetCheck {
    Ok,
    BelowTableMin,
    AboveTableMax,
    NotEnoughPlayerMoney,
    CasinoCannotCover,
};

std::string toString(BetCheck check);

class BetRejected : public std::runtime_error {
public:
    explicit BetRejected(BetCheck reason);
    BetCheck reason() const { return reason_; }

private:
    BetCheck reason_;
};

struct BetOutcome {
    Bet bet;
    bool won;
    Money payout;
};

struct RoundResult {
    Pocket pocket;
    std::vector<BetOutcome> outcomes;
};

class Round {
public:
    Round(Player& player, Cashbox& cashbox, TableLimits limits);

    BetCheck check(const Bet& bet) const;

    void place(const Bet& bet);

    RoundResult settle(const Pocket& pocket);

    const std::vector<Bet>& bets() const { return bets_; }
    bool isSettled() const { return settled_; }

private:
    Player& player_;
    Cashbox& cashbox_;
    TableLimits limits_;
    std::vector<Bet> bets_;
    bool settled_ = false;
};

}
