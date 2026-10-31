#include <gtest/gtest.h>

#include "FakeGenerator.hpp"
#include "application/RouletteService.hpp"
#include "infrastructure/Mt19937Generator.hpp"

using namespace casino;

TEST(RouletteServiceTest, SpinWithoutBets) {
    FakeGenerator generator({1});
    RouletteService service(Player("Анна", Money(100)), Cashbox(Money(1000)),
                            TableLimits(Money(1), Money(50)), generator);
    EXPECT_THROW(service.spin(), std::logic_error);
}

TEST(RouletteServiceTest, SpinAndJournal) {
    FakeGenerator generator({1});
    RouletteService service(Player("Анна", Money(100)), Cashbox(Money(1000)),
                            TableLimits(Money(1), Money(50)), generator);
    service.placeBet(Bet::onColor(Color::Red, Money(10)));
    service.spin();

    EXPECT_EQ(service.player().balance(), Money(110));
    EXPECT_EQ(service.cashbox().balance(), Money(990));
    EXPECT_EQ(service.spinCount(), 1);
    ASSERT_EQ(service.journal().size(), 1);
    EXPECT_EQ(service.journal()[0].pocket, 1);
    EXPECT_EQ(service.journal()[0].payout, 20);
}

TEST(RouletteServiceTest, SameSeedSameGame) {
    Mt19937Generator first(42);
    Mt19937Generator second(42);
    RouletteService a(Player("Анна", Money(100)), Cashbox(Money(1000)),
                      TableLimits(Money(1), Money(50)), first);
    RouletteService b(Player("Анна", Money(100)), Cashbox(Money(1000)),
                      TableLimits(Money(1), Money(50)), second);
    for (int i = 0; i < 20; ++i) {
        a.placeBet(Bet::onColor(Color::Red, Money(1)));
        b.placeBet(Bet::onColor(Color::Red, Money(1)));
        a.spin();
        b.spin();
    }
    EXPECT_EQ(a.journal(), b.journal());
}
