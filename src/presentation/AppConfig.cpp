#include "presentation/AppConfig.hpp"

#include <limits>
#include <stdexcept>

namespace casino {

namespace {

long long toNumber(const std::string& key, const std::string& value) {
    const std::string error = "Неверное значение для " + key + ": " + value;
    std::size_t parsedLength = 0;
    long long number = 0;
    try {
        number = std::stoll(value, &parsedLength);
    } catch (const std::logic_error&) {
        throw std::invalid_argument(error);
    }
    if (parsedLength != value.size() || number < 0) {
        throw std::invalid_argument(error);
    }
    return number;
}

}

AppConfig parseArgs(const std::vector<std::string>& args) {
    AppConfig config;
    for (std::size_t i = 0; i < args.size(); ++i) {
        const std::string& key = args[i];
        if (key == "--help") {
            config.showHelp = true;
            continue;
        }
        if (i + 1 >= args.size()) {
            throw std::invalid_argument("После " + key + " нужно указать значение");
        }
        const std::string& value = args[++i];

        if (key == "--mode") {
            if (value == "manual") {
                config.mode = Mode::Manual;
            } else if (value == "bot") {
                config.mode = Mode::Bot;
            } else {
                throw std::invalid_argument("Режим может быть manual или bot, а не " + value);
            }
        } else if (key == "--seed") {
            long long seed = toNumber(key, value);
            if (seed > std::numeric_limits<std::uint32_t>::max()) {
                throw std::invalid_argument("Слишком большой seed: " + value);
            }
            config.seed = static_cast<std::uint32_t>(seed);
        } else if (key == "--name") {
            config.playerName = value;
        } else if (key == "--player") {
            config.playerMoney = toNumber(key, value);
        } else if (key == "--cashbox") {
            config.cashboxMoney = toNumber(key, value);
        } else if (key == "--min") {
            config.tableMin = toNumber(key, value);
        } else if (key == "--max") {
            config.tableMax = toNumber(key, value);
        } else if (key == "--strategy") {
            config.strategy = value;
        } else if (key == "--bet") {
            config.baseBet = toNumber(key, value);
        } else if (key == "--spins") {
            long long spins = toNumber(key, value);
            if (spins > std::numeric_limits<int>::max()) {
                throw std::invalid_argument("Слишком большое число вращений: " + value);
            }
            config.spins = static_cast<int>(spins);
        } else if (key == "--loss-limit") {
            config.lossLimit = toNumber(key, value);
        } else if (key == "--win-goal") {
            config.winGoal = toNumber(key, value);
        } else if (key == "--journal") {
            config.journalPath = value;
        } else {
            throw std::invalid_argument("Неизвестный параметр: " + key);
        }
    }
    return config;
}

std::string usage() {
    return "Симулятор европейской рулетки\n"
           "\n"
           "Использование: casino [параметры]\n"
           "\n"
           "  --mode manual|bot     ручная игра или бот (по умолчанию manual)\n"
           "  --seed N              начальное значение генератора (42)\n"
           "  --name ИМЯ            имя игрока (Игрок)\n"
           "  --player N            стартовый счёт игрока (1000)\n"
           "  --cashbox N           стартовая касса казино (100000)\n"
           "  --min N, --max N      лимиты ставки стола (1 и 100)\n"
           "  --journal ФАЙЛ        файл журнала (journal.csv)\n"
           "\n"
           "Только для бота:\n"
           "  --strategy flat|martingale   стратегия (flat)\n"
           "  --bet N               базовая ставка (10)\n"
           "  --spins N             максимум вращений (100)\n"
           "  --loss-limit N        не проигрывать больше N (0 = не использовать)\n"
           "  --win-goal N          остановиться при выигрыше N (0 = не использовать)\n"
           "\n"
           "Пример: casino --mode bot --seed 7 --strategy martingale --spins 200 --loss-limit 300\n";
}

}
