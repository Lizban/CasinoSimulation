#include "domain/TableLimits.hpp"

#include <stdexcept>

namespace casino {

TableLimits::TableLimits(Money min, Money max) : min_(min), max_(max) {
    if (min.chips() == 0) {
        throw std::invalid_argument("Минимальная ставка должна быть больше нуля");
    }
    if (min > max) {
        throw std::invalid_argument("Минимальная ставка больше максимальной");
    }
}

bool TableLimits::allows(Money amount) const {
    return amount >= min_ && amount <= max_;
}

}
