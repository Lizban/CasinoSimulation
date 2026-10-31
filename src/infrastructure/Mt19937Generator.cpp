#include "infrastructure/Mt19937Generator.hpp"

#include <stdexcept>

namespace casino {

Mt19937Generator::Mt19937Generator(std::uint32_t seed) : engine_(seed) {}

int Mt19937Generator::nextInt(int min, int max) {
    if (min > max) {
        throw std::invalid_argument("Нижняя граница больше верхней");
    }
    const std::uint64_t range = static_cast<std::uint64_t>(static_cast<std::int64_t>(max) - min) + 1;
    const std::uint64_t engineRange = 0x100000000ULL;

    const std::uint64_t limit = engineRange - engineRange % range;
    std::uint64_t value = engine_();
    while (value >= limit) {
        value = engine_();
    }
    return static_cast<int>(min + static_cast<std::int64_t>(value % range));
}

}
