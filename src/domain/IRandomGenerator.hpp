#pragma once

namespace casino {

class IRandomGenerator {
public:
    virtual ~IRandomGenerator() = default;

    virtual int nextInt(int min, int max) = 0;
};

}
