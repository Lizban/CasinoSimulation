#pragma once

#include "domain/IRandomGenerator.hpp"
#include "domain/Pocket.hpp"

namespace casino {

class RouletteWheel {
public:
    explicit RouletteWheel(IRandomGenerator& generator);

    Pocket spin();

private:
    IRandomGenerator& generator_;
};

}
