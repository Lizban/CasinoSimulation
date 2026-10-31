#include "application/RouletteService.hpp"

#include <stdexcept>
#include <utility>

namespace casino {

RouletteService::RouletteService(Player player, Cashbox cashbox, TableLimits limits,
                                 IRandomGenerator& generator)
    : player_(std::move(player)), cashbox_(cashbox), limits_(limits), wheel_(generator) {
    round_.emplace(player_, cashbox_, limits_);
}

BetCheck RouletteService::checkBet(const Bet& bet) const {
    return round_->check(bet);
}

void RouletteService::placeBet(const Bet& bet) {
    round_->place(bet);
}

const std::vector<Bet>& RouletteService::pendingBets() const {
    return round_->bets();
}

RoundResult RouletteService::spin() {
    if (round_->bets().empty()) {
        throw std::logic_error("Перед вращением нужно сделать хотя бы одну ставку");
    }
    Pocket pocket = wheel_.spin();
    RoundResult result = round_->settle(pocket);
    ++spinCount_;
    writeToJournal(result);
    round_.emplace(player_, cashbox_, limits_);
    return result;
}

GameOverReason RouletteService::gameOver() const {
    return checkGameOver(player_, cashbox_, limits_);
}

void RouletteService::writeToJournal(const RoundResult& result) {
    for (const BetOutcome& outcome : result.outcomes) {
        JournalEntry entry;
        entry.spin = spinCount_;
        entry.betType = toString(outcome.bet.type());
        entry.target = outcome.bet.targetName();
        entry.amount = outcome.bet.amount().chips();
        entry.pocket = result.pocket.number();
        entry.pocketColor = toString(result.pocket.color());
        entry.payout = outcome.payout.chips();
        entry.playerBalance = player_.balance().chips();
        entry.cashboxBalance = cashbox_.balance().chips();
        journal_.push_back(entry);
    }
}

}
