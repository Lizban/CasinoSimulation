#include "domain/Pocket.hpp"

#include <set>
#include <stdexcept>

namespace casino {

namespace {

const std::set<int> kRedNumbers = {1,  3,  5,  7,  9,  12, 14, 16, 18,
                                   19, 21, 23, 25, 27, 30, 32, 34, 36};

}

std::string toString(Color color) {
    switch (color) {
        case Color::Green: return "green";
        case Color::Red: return "red";
        case Color::Black: return "black";
    }
    return "unknown";
}

Pocket::Pocket(int number) : number_(number) {
    if (!isValidNumber(number)) {
        throw std::invalid_argument("Номер кармана должен быть от 0 до 36");
    }
}

Color Pocket::color() const {
    if (number_ == 0) {
        return Color::Green;
    }
    return kRedNumbers.count(number_) > 0 ? Color::Red : Color::Black;
}

bool Pocket::isValidNumber(int number) {
    return number >= kMinNumber && number <= kMaxNumber;
}

}
