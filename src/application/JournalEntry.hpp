#pragma once

#include <string>

namespace casino {

struct JournalEntry {
    int spin = 0;
    std::string betType;
    std::string target;
    long long amount = 0;
    int pocket = 0;
    std::string pocketColor;
    long long payout = 0;
    long long playerBalance = 0;
    long long cashboxBalance = 0;

    bool operator==(const JournalEntry& other) const {
        return spin == other.spin && betType == other.betType && target == other.target &&
               amount == other.amount && pocket == other.pocket &&
               pocketColor == other.pocketColor && payout == other.payout &&
               playerBalance == other.playerBalance && cashboxBalance == other.cashboxBalance;
    }
};

}
