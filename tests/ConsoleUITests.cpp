#include <gtest/gtest.h>

#include <sstream>

#include "FakeGenerator.hpp"
#include "InMemoryJournalRepository.hpp"
#include "presentation/ConsoleUI.hpp"

using namespace casino;

TEST(ConsoleUITest, ColorBetAndSpin) {
    FakeGenerator generator({1});
    RouletteService service(Player("Анна", Money(100)), Cashbox(Money(1000)),
                            TableLimits(Money(1), Money(50)), generator);
    InMemoryJournalRepository repository;
    std::istringstream in("1\n1\n10\n4\n3\n0\n");
    std::ostringstream out;
    ConsoleUI ui(service, repository, in, out);
    ui.run();

    EXPECT_NE(out.str().find("Выпало: 1"), std::string::npos);
    EXPECT_EQ(service.player().balance(), Money(110));
}

TEST(ConsoleUITest, NumberBetSaveAndLoad) {
    FakeGenerator generator({17});
    RouletteService service(Player("Анна", Money(100)), Cashbox(Money(1000)),
                            TableLimits(Money(1), Money(50)), generator);
    InMemoryJournalRepository repository;
    std::istringstream in("2\n17\n5\n3\n5\n6\n0\n");
    std::ostringstream out;
    ConsoleUI ui(service, repository, in, out);
    ui.run();

    EXPECT_EQ(repository.saved.size(), 1);
    EXPECT_NE(out.str().find("В журнале записей: 1"), std::string::npos);
}

TEST(ConsoleUITest, WrongInput) {
    FakeGenerator generator({1});
    RouletteService service(Player("Анна", Money(100)), Cashbox(Money(1000)),
                            TableLimits(Money(1), Money(50)), generator);
    InMemoryJournalRepository repository;
    std::istringstream in("abc\n9\n3\n1\n1\n60\n0\n");
    std::ostringstream out;
    ConsoleUI ui(service, repository, in, out);
    ui.run();

    EXPECT_NE(out.str().find("Нужно ввести целое число"), std::string::npos);
    EXPECT_NE(out.str().find("Сначала сделайте ставку"), std::string::npos);
    EXPECT_NE(out.str().find("Ставка отклонена"), std::string::npos);
}

TEST(ConsoleUITest, BotReport) {
    BotReport report;
    report.spins = 5;
    report.startBalance = Money(100);
    report.finalBalance = Money(150);
    std::ostringstream out;
    printBotReport(out, "flat", report);
    EXPECT_NE(out.str().find("(+50)"), std::string::npos);
}
