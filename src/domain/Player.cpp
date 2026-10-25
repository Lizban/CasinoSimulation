#include "domain/Player.hpp"

#include <stdexcept>
#include <utility>

namespace casino {

Player::Player(std::string name, Money balance) : name_(std::move(name)), balance_(balance) {
    if (name_.empty()) {
        throw std::invalid_argument("У игрока должно быть имя");
    }
}

bool Player::canAfford(Money amount) const {
    return amount <= balance_;
}

void Player::withdraw(Money amount) {
    if (!canAfford(amount)) {
        throw std::logic_error("На счёте игрока недостаточно денег");
    }
    balance_ = balance_ - amount;
}

void Player::deposit(Money amount) {
    balance_ = balance_ + amount;
}

}
