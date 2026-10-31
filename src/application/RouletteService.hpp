#pragma once

#include <optional>
#include <vector>

#include "application/JournalEntry.hpp"
#include "domain/Cashbox.hpp"
#include "domain/GameRules.hpp"
#include "domain/IRandomGenerator.hpp"
#include "domain/Player.hpp"
#include "domain/Round.hpp"
#include "domain/RouletteWheel.hpp"
#include "domain/TableLimits.hpp"

namespace casino {

class RouletteService {
public:
    RouletteService(Player player, Cashbox cashbox, TableLimits limits,
                    IRandomGenerator& generator);

    RouletteService(const RouletteService&) = delete;
    RouletteService& operator=(const RouletteService&) = delete;

    BetCheck checkBet(const Bet& bet) const;
    void placeBet(const Bet& bet);
    const std::vector<Bet>& pendingBets() const;

    RoundResult spin();

    const Player& player() const { return player_; }
    const Cashbox& cashbox() const { return cashbox_; }
    const TableLimits& limits() const { return limits_; }
    int spinCount() const { return spinCount_; }
    GameOverReason gameOver() const;

    const std::vector<JournalEntry>& journal() const { return journal_; }

private:
    void writeToJournal(const RoundResult& result);

    Player player_;
    Cashbox cashbox_;
    TableLimits limits_;
    RouletteWheel wheel_;
    std::optional<Round> round_;
    int spinCount_ = 0;
    std::vector<JournalEntry> journal_;
};

}
