#pragma once

#include "domain/Money.hpp"

namespace casino {

class TableLimits {
public:
    TableLimits(Money min, Money max);

    Money min() const { return min_; }
    Money max() const { return max_; }

    bool allows(Money amount) const;

private:
    Money min_;
    Money max_;
};

}
