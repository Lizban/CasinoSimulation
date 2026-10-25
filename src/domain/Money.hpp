#pragma once

#include <cstdint>
#include <iosfwd>

namespace casino {

class Money {
public:
    Money() = default;
    explicit Money(std::int64_t chips);

    std::int64_t chips() const { return chips_; }

    Money operator+(Money other) const;
    Money operator-(Money other) const;
    Money operator*(int factor) const;

    bool operator==(Money other) const { return chips_ == other.chips_; }
    bool operator!=(Money other) const { return chips_ != other.chips_; }
    bool operator<(Money other) const { return chips_ < other.chips_; }
    bool operator<=(Money other) const { return chips_ <= other.chips_; }
    bool operator>(Money other) const { return chips_ > other.chips_; }
    bool operator>=(Money other) const { return chips_ >= other.chips_; }

private:
    std::int64_t chips_ = 0;
};

std::ostream& operator<<(std::ostream& out, Money money);

}
