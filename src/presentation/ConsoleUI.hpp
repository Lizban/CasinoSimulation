#pragma once

#include <iosfwd>
#include <string>

#include "application/BotRunner.hpp"
#include "application/IJournalRepository.hpp"
#include "application/RouletteService.hpp"

namespace casino {

class ConsoleUI {
public:
    ConsoleUI(RouletteService& service, IJournalRepository& repository, std::istream& in,
              std::ostream& out);

    void run();

private:
    void printMenu();
    void printStatus();
    void betOnColor();
    void betOnNumber();
    void placeBet(const Bet& bet);
    void spin();
    void saveJournal();
    void loadJournal();

    bool readNumber(const std::string& prompt, long long& value);

    RouletteService& service_;
    IJournalRepository& repository_;
    std::istream& in_;
    std::ostream& out_;
};

void printBotReport(std::ostream& out, const std::string& strategyName, const BotReport& report);

}
