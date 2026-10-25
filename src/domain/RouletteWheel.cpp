#include "domain/RouletteWheel.hpp"

namespace casino {

RouletteWheel::RouletteWheel(IRandomGenerator& generator) : generator_(generator) {}

Pocket RouletteWheel::spin() {
    return Pocket(generator_.nextInt(Pocket::kMinNumber, Pocket::kMaxNumber));
}

}
