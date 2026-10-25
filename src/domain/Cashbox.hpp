#pragma once

#include "domain/Money.hpp"

namespace casino {

class Cashbox {
public:
    explicit Cashbox(Money balance);

    Money balance() const { return balance_; }

    bool canPay(Money amount) const;
    void receive(Money amount);
    void pay(Money amount);

private:
    Money balance_;
};

}
