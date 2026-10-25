#pragma once

#include <string>

#include "domain/Cashbox.hpp"
#include "domain/Player.hpp"
#include "domain/TableLimits.hpp"

namespace casino {

enum class GameOverReason {
    None,
    PlayerCannotBet,
    CasinoCannotPay,
};

GameOverReason checkGameOver(const Player& player, const Cashbox& cashbox,
                             const TableLimits& limits);

std::string toString(GameOverReason reason);

}
