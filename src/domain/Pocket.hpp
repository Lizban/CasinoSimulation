#pragma once

#include <string>

namespace casino {

enum class Color { Green, Red, Black };

std::string toString(Color color);

class Pocket {
public:
    static const int kMinNumber = 0;
    static const int kMaxNumber = 36;

    explicit Pocket(int number);

    int number() const { return number_; }
    Color color() const;

    static bool isValidNumber(int number);

private:
    int number_;
};

}
