#include "domain/Round.hpp"

namespace casino {

namespace {

Money worstCaseWinnings(const std::vector<Bet>& bets) {
    Money worst;
    for (int number = Pocket::kMinNumber; number <= Pocket::kMaxNumber; ++number) {
        Pocket pocket(number);
        Money total;
        for (const Bet& bet : bets) {
            if (bet.winsOn(pocket)) {
                total = total + bet.winnings();
            }
        }
        if (total > worst) {
            worst = total;
        }
    }
    return worst;
}

}

std::string toString(BetCheck check) {
    switch (check) {
        case BetCheck::Ok: return "ставка допустима";
        case BetCheck::BelowTableMin: return "ставка меньше минимальной ставки стола";
        case BetCheck::AboveTableMax: return "ставка больше максимальной ставки стола";
        case BetCheck::NotEnoughPlayerMoney: return "на счёте игрока недостаточно денег";
        case BetCheck::CasinoCannotCover: return "в кассе не хватит денег на возможный выигрыш";
    }
    return "неизвестная причина";
}

BetRejected::BetRejected(BetCheck reason)
    : std::runtime_error("Ставка отклонена: " + toString(reason)), reason_(reason) {}

Round::Round(Player& player, Cashbox& cashbox, TableLimits limits)
    : player_(player), cashbox_(cashbox), limits_(limits) {}

BetCheck Round::check(const Bet& bet) const {
    if (bet.amount() < limits_.min()) {
        return BetCheck::BelowTableMin;
    }
    if (bet.amount() > limits_.max()) {
        return BetCheck::AboveTableMax;
    }
    if (!player_.canAfford(bet.amount())) {
        return BetCheck::NotEnoughPlayerMoney;
    }
    std::vector<Bet> withNewBet = bets_;
    withNewBet.push_back(bet);
    if (!cashbox_.canPay(worstCaseWinnings(withNewBet))) {
        return BetCheck::CasinoCannotCover;
    }
    return BetCheck::Ok;
}

void Round::place(const Bet& bet) {
    if (settled_) {
        throw std::logic_error("Раунд уже рассчитан, ставки больше не принимаются");
    }
    BetCheck result = check(bet);
    if (result != BetCheck::Ok) {
        throw BetRejected(result);
    }
    player_.withdraw(bet.amount());
    bets_.push_back(bet);
}

RoundResult Round::settle(const Pocket& pocket) {
    if (settled_) {
        throw std::logic_error("Раунд уже рассчитан");
    }
    if (bets_.empty()) {
        throw std::logic_error("Нельзя рассчитать раунд без ставок");
    }
    settled_ = true;

    RoundResult result{pocket, {}};
    for (const Bet& bet : bets_) {
        if (bet.winsOn(pocket)) {
            cashbox_.pay(bet.winnings());
            Money payout = bet.amount() + bet.winnings();
            player_.deposit(payout);
            result.outcomes.push_back({bet, true, payout});
        } else {
            cashbox_.receive(bet.amount());
            result.outcomes.push_back({bet, false, Money()});
        }
    }
    return result;
}

}
