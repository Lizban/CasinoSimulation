#include "application/BotRunner.hpp"

#include <stdexcept>

namespace casino {

std::string toString(BotStopReason reason) {
    switch (reason) {
        case BotStopReason::SpinLimit: return "сделано заданное число вращений";
        case BotStopReason::LossLimit: return "достигнут лимит потерь";
        case BotStopReason::WinGoal: return "достигнута цель по выигрышу";
        case BotStopReason::GameOver: return "игра окончена";
        case BotStopReason::NoValidBet: return "нет допустимой ставки";
    }
    return "неизвестная причина";
}

BotReport runBot(RouletteService& service, IBettingStrategy& strategy, const StopRules& rules) {
    if (rules.maxSpins <= 0) {
        throw std::invalid_argument("Число вращений должно быть больше нуля");
    }

    BotReport report;
    report.startBalance = service.player().balance();

    while (true) {
        Money balance = service.player().balance();

        if (rules.winGoal.chips() > 0 && balance >= report.startBalance + rules.winGoal) {
            report.reason = BotStopReason::WinGoal;
            break;
        }
        if (service.gameOver() != GameOverReason::None) {
            report.reason = BotStopReason::GameOver;
            break;
        }
        if (report.spins >= rules.maxSpins) {
            report.reason = BotStopReason::SpinLimit;
            break;
        }

        Bet bet = strategy.nextBet(balance, service.limits());

        if (rules.lossLimit.chips() > 0 &&
            report.startBalance + bet.amount() > balance + rules.lossLimit) {
            report.reason = BotStopReason::LossLimit;
            break;
        }
        if (service.checkBet(bet) != BetCheck::Ok) {
            report.reason = BotStopReason::NoValidBet;
            break;
        }
        service.placeBet(bet);
        strategy.onResult(service.spin());
        ++report.spins;
    }

    report.finalBalance = service.player().balance();
    return report;
}

}
