#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace casino {

enum class Mode { Manual, Bot };

struct AppConfig {
    Mode mode = Mode::Manual;
    std::uint32_t seed = 42;
    std::string playerName = "Игрок";
    long long playerMoney = 1000;
    long long cashboxMoney = 100000;
    long long tableMin = 1;
    long long tableMax = 100;
    std::string strategy = "flat";
    long long baseBet = 10;
    int spins = 100;
    long long lossLimit = 0;
    long long winGoal = 0;
    std::string journalPath = "journal.csv";
    bool showHelp = false;
};

AppConfig parseArgs(const std::vector<std::string>& args);

std::string usage();

}
