#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "application/BettingStrategies.hpp"
#include "application/BotRunner.hpp"
#include "application/RouletteService.hpp"
#include "infrastructure/CsvJournalRepository.hpp"
#include "infrastructure/Mt19937Generator.hpp"
#include "presentation/AppConfig.hpp"
#include "presentation/ConsoleUI.hpp"

using namespace casino;

int main(int argc, char* argv[]) {
    try {
        AppConfig config = parseArgs(std::vector<std::string>(argv + 1, argv + argc));
        if (config.showHelp) {
            std::cout << usage();
            return 0;
        }

        Mt19937Generator generator(config.seed);
        CsvJournalRepository repository(config.journalPath);
        RouletteService service(Player(config.playerName, Money(config.playerMoney)),
                                Cashbox(Money(config.cashboxMoney)),
                                TableLimits(Money(config.tableMin), Money(config.tableMax)),
                                generator);

        if (config.mode == Mode::Manual) {
            ConsoleUI ui(service, repository, std::cin, std::cout);
            ui.run();
            return 0;
        }

        std::unique_ptr<IBettingStrategy> strategy = makeStrategy(config.strategy, Money(config.baseBet));
        StopRules rules{config.spins, Money(config.lossLimit), Money(config.winGoal)};
        BotReport report = runBot(service, *strategy, rules);
        repository.save(service.journal());

        std::cout << "Seed: " << config.seed << '\n';
        printBotReport(std::cout, strategy->name(), report);
        std::cout << "Журнал сохранён в " << config.journalPath << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Ошибка: " << error.what() << "\nСправка: casino --help\n";
        return 1;
    }
}
