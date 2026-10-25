#include "domain/GameRules.hpp"

namespace casino {

GameOverReason checkGameOver(const Player& player, const Cashbox& cashbox,
                             const TableLimits& limits) {
    if (player.balance() < limits.min()) {
        return GameOverReason::PlayerCannotBet;
    }
    if (!cashbox.canPay(limits.min())) {
        return GameOverReason::CasinoCannotPay;
    }
    return GameOverReason::None;
}

std::string toString(GameOverReason reason) {
    switch (reason) {
        case GameOverReason::None: return "игра продолжается";
        case GameOverReason::PlayerCannotBet: return "у игрока не хватает денег даже на минимальную ставку";
        case GameOverReason::CasinoCannotPay: return "в кассе казино не хватает денег на выплаты";
    }
    return "неизвестная причина";
}

}
