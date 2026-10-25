#pragma once

#include <string>

#include "domain/Money.hpp"

namespace casino {

class Player {
public:
    Player(std::string name, Money balance);

    const std::string& name() const { return name_; }
    Money balance() const { return balance_; }

    bool canAfford(Money amount) const;
    void withdraw(Money amount);
    void deposit(Money amount);

private:
    std::string name_;
    Money balance_;
};

}
