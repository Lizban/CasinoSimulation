#pragma once

#include <string>

#include "application/BettingStrategies.hpp"
#include "application/RouletteService.hpp"
#include "domain/Money.hpp"

namespace casino {

struct StopRules {
    int maxSpins = 100;
    Money lossLimit;
    Money winGoal;
};

enum class BotStopReason {
    SpinLimit,
    LossLimit,
    WinGoal,
    GameOver,
    NoValidBet,
};

std::string toString(BotStopReason reason);

struct BotReport {
    int spins = 0;
    BotStopReason reason = BotStopReason::SpinLimit;
    Money startBalance;
    Money finalBalance;
};

BotReport runBot(RouletteService& service, IBettingStrategy& strategy, const StopRules& rules);

}
