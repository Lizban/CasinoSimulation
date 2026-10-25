#include "domain/Cashbox.hpp"

#include <stdexcept>

namespace casino {

Cashbox::Cashbox(Money balance) : balance_(balance) {}

bool Cashbox::canPay(Money amount) const {
    return amount <= balance_;
}

void Cashbox::receive(Money amount) {
    balance_ = balance_ + amount;
}

void Cashbox::pay(Money amount) {
    if (!canPay(amount)) {
        throw std::logic_error("В кассе казино недостаточно денег для выплаты");
    }
    balance_ = balance_ - amount;
}

}
