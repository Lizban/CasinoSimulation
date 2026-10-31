#pragma once

#include <cstdint>
#include <random>

#include "domain/IRandomGenerator.hpp"

namespace casino {

class Mt19937Generator : public IRandomGenerator {
public:
    explicit Mt19937Generator(std::uint32_t seed);

    int nextInt(int min, int max) override;

private:
    std::mt19937 engine_;
};

}
