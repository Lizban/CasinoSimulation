#pragma once

#include <cstddef>
#include <utility>
#include <vector>

#include "domain/IRandomGenerator.hpp"

namespace casino {

class FakeGenerator : public IRandomGenerator {
public:
    explicit FakeGenerator(std::vector<int> numbers) : numbers_(std::move(numbers)) {}

    int nextInt(int, int) override {
        int value = numbers_[index_ % numbers_.size()];
        ++index_;
        return value;
    }

private:
    std::vector<int> numbers_;
    std::size_t index_ = 0;
};

}
