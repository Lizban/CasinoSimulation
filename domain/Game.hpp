#pragma once
#include <memory>
#include "Bet.hpp"
#include "Casino.hpp"
#include "Player.hpp"

namespace casino::domain{
class Game final{
private:
    enum class Outcome { Zero, Red, Black };
    int spin();
    Money calculatePayout(Outcome outcome, const Bet& bet) const;
    std::unique_ptr<Player> player;
    std::unique_ptr<Casino> casino;
public:
    explicit Game(std::unique_ptr<Player> player, std::unique_ptr<Casino> casino);
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) noexcept = default;
    Game& operator=(Game&&) noexcept = default;

    ~Game() = default;

    [[nodiscard]] bool canPlaceBets(const Bet& colorBet, const Bet& numberBet) const noexcept;

    void placeColorBet(Bet bet);
    bool placeNumberBet(int number, Bet bet);
};
}