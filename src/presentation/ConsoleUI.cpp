#include "presentation/ConsoleUI.hpp"

#include <istream>
#include <ostream>
#include <stdexcept>

namespace casino {

namespace {

std::string colorName(Color color) {
    switch (color) {
        case Color::Green: return "зеро";
        case Color::Red: return "красное";
        case Color::Black: return "чёрное";
    }
    return "?";
}

std::string describe(const Bet& bet) {
    std::string target = bet.type() == BetType::Color ? colorName(bet.color())
                                                      : "число " + std::to_string(bet.number());
    return std::to_string(bet.amount().chips()) + " фишек на " + target;
}

const std::size_t kJournalRowsToShow = 10;

}

ConsoleUI::ConsoleUI(RouletteService& service, IJournalRepository& repository, std::istream& in,
                     std::ostream& out)
    : service_(service), repository_(repository), in_(in), out_(out) {}

void ConsoleUI::run() {
    out_ << "Европейская рулетка. Ставки от " << service_.limits().min() << " до "
         << service_.limits().max() << " фишек.\n";
    printStatus();

    while (true) {
        GameOverReason over = service_.gameOver();
        if (over != GameOverReason::None && service_.pendingBets().empty()) {
            out_ << "Игра окончена: " << toString(over) << ".\n";
            return;
        }

        printMenu();
        long long choice = 0;
        if (!readNumber("Ваш выбор: ", choice)) {
            return;
        }
        switch (choice) {
            case 1: betOnColor(); break;
            case 2: betOnNumber(); break;
            case 3: spin(); break;
            case 4: printStatus(); break;
            case 5: saveJournal(); break;
            case 6: loadJournal(); break;
            case 0: out_ << "До свидания!\n"; return;
            default: out_ << "Нет такого пункта меню.\n";
        }
    }
}

void ConsoleUI::printMenu() {
    out_ << "\n1 - ставка на цвет\n"
            "2 - ставка на число\n"
            "3 - крутить рулетку\n"
            "4 - показать счета\n"
            "5 - сохранить журнал\n"
            "6 - загрузить журнал из файла\n"
            "0 - выход\n";
}

void ConsoleUI::printStatus() {
    out_ << "Счёт игрока " << service_.player().name() << ": " << service_.player().balance()
         << " фишек. Касса казино: " << service_.cashbox().balance() << " фишек.\n";
    const std::vector<Bet>& bets = service_.pendingBets();
    if (!bets.empty()) {
        out_ << "Ставки на столе:\n";
        for (const Bet& bet : bets) {
            out_ << "  " << describe(bet) << '\n';
        }
    }
}

void ConsoleUI::betOnColor() {
    long long colorChoice = 0;
    long long amount = 0;
    if (!readNumber("Цвет (1 - красное, 2 - чёрное): ", colorChoice)) {
        return;
    }
    if (colorChoice != 1 && colorChoice != 2) {
        out_ << "Нужно выбрать 1 или 2.\n";
        return;
    }
    if (!readNumber("Сумма ставки: ", amount)) {
        return;
    }
    try {
        Color color = colorChoice == 1 ? Color::Red : Color::Black;
        placeBet(Bet::onColor(color, Money(amount)));
    } catch (const std::invalid_argument& error) {
        out_ << error.what() << ".\n";
    }
}

void ConsoleUI::betOnNumber() {
    long long number = 0;
    long long amount = 0;
    if (!readNumber("Число (0-36): ", number)) {
        return;
    }
    if (number < Pocket::kMinNumber || number > Pocket::kMaxNumber) {
        out_ << "Число должно быть от 0 до 36.\n";
        return;
    }
    if (!readNumber("Сумма ставки: ", amount)) {
        return;
    }
    try {
        placeBet(Bet::onNumber(static_cast<int>(number), Money(amount)));
    } catch (const std::invalid_argument& error) {
        out_ << error.what() << ".\n";
    }
}

void ConsoleUI::placeBet(const Bet& bet) {
    try {
        service_.placeBet(bet);
        out_ << "Ставка принята: " << describe(bet) << ".\n";
    } catch (const BetRejected& error) {
        out_ << error.what() << ".\n";
    }
}

void ConsoleUI::spin() {
    if (service_.pendingBets().empty()) {
        out_ << "Сначала сделайте ставку.\n";
        return;
    }
    RoundResult result = service_.spin();
    out_ << "Выпало: " << result.pocket.number() << " (" << colorName(result.pocket.color())
         << ").\n";
    for (const BetOutcome& outcome : result.outcomes) {
        if (outcome.won) {
            out_ << "  " << describe(outcome.bet) << ": выигрыш, вам выплачено " << outcome.payout
                 << ".\n";
        } else {
            out_ << "  " << describe(outcome.bet) << ": проигрыш.\n";
        }
    }
    printStatus();
}

void ConsoleUI::saveJournal() {
    try {
        repository_.save(service_.journal());
        out_ << "Журнал сохранён, записей: " << service_.journal().size() << ".\n";
    } catch (const std::exception& error) {
        out_ << "Ошибка: " << error.what() << ".\n";
    }
}

void ConsoleUI::loadJournal() {
    try {
        std::vector<JournalEntry> entries = repository_.load();
        out_ << "В журнале записей: " << entries.size() << ".\n";
        std::size_t first = entries.size() > kJournalRowsToShow ? entries.size() - kJournalRowsToShow : 0;
        if (first > 0) {
            out_ << "Последние " << kJournalRowsToShow << ":\n";
        }
        for (std::size_t i = first; i < entries.size(); ++i) {
            const JournalEntry& e = entries[i];
            out_ << "  #" << e.spin << ": " << e.amount << " на " << e.target << ", выпало "
                 << e.pocket << ", выплата " << e.payout << ", игрок " << e.playerBalance
                 << ", касса " << e.cashboxBalance << '\n';
        }
    } catch (const std::exception& error) {
        out_ << "Ошибка: " << error.what() << ".\n";
    }
}

bool ConsoleUI::readNumber(const std::string& prompt, long long& value) {
    std::string line;
    while (true) {
        out_ << prompt;
        if (!std::getline(in_, line)) {
            out_ << '\n';
            return false;
        }
        try {
            std::size_t parsedLength = 0;
            value = std::stoll(line, &parsedLength);
            if (parsedLength == line.size()) {
                return true;
            }
        } catch (const std::logic_error&) {
        }
        out_ << "Нужно ввести целое число.\n";
    }
}

void printBotReport(std::ostream& out, const std::string& strategyName, const BotReport& report) {
    long long result = report.finalBalance.chips() - report.startBalance.chips();
    out << "Стратегия: " << strategyName << '\n'
        << "Вращений: " << report.spins << '\n'
        << "Причина остановки: " << toString(report.reason) << '\n'
        << "Баланс: " << report.startBalance << " -> " << report.finalBalance << " ("
        << (result >= 0 ? "+" : "") << result << ")\n";
}

}
