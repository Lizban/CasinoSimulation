#pragma once
#include <stdexcept>
#include <cmath>

namespace casino::domain{
class Money final{
private:
    const double amount;
public:
    explicit Money(double amount);
    Money(const Money&) = default;
    Money(Money&&) noexcept = default; 
    Money& operator=(const Money&) = default;
    Money& operator=(Money&&) noexcept = default;

    ~Money() = default;

    [[nodiscard]] double getAmount() const { return amount; }

    void add(Money other);
    void subtract(Money other);
    Money multiply(int multiplier) const;
};
}