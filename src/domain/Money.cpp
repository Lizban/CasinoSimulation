#include "domain/Money.hpp"

#include <ostream>
#include <stdexcept>

namespace casino {

Money::Money(std::int64_t chips) : chips_(chips) {
    if (chips < 0) {
        throw std::invalid_argument("Сумма денег не может быть отрицательной");
    }
}

Money Money::operator+(Money other) const {
    return Money(chips_ + other.chips_);
}

Money Money::operator-(Money other) const {
    if (other.chips_ > chips_) {
        throw std::invalid_argument("После вычитания сумма стала бы отрицательной");
    }
    return Money(chips_ - other.chips_);
}

Money Money::operator*(int factor) const {
    if (factor < 0) {
        throw std::invalid_argument("Деньги нельзя умножать на отрицательное число");
    }
    return Money(chips_ * factor);
}

std::ostream& operator<<(std::ostream& out, Money money) {
    return out << money.chips();
}

}
