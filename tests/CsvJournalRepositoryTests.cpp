#include <gtest/gtest.h>

#include <cstdio>

#include "infrastructure/CsvJournalRepository.hpp"

using namespace casino;

TEST(CsvJournalRepositoryTest, SaveAndLoad) {
    JournalEntry entry;
    entry.spin = 1;
    entry.betType = "color";
    entry.target = "red";
    entry.amount = 10;
    entry.pocket = 1;
    entry.pocketColor = "red";
    entry.payout = 20;
    entry.playerBalance = 110;
    entry.cashboxBalance = 990;

    CsvJournalRepository repository("test_journal.csv");
    repository.save({entry, entry});
    std::vector<JournalEntry> loaded = repository.load();
    std::remove("test_journal.csv");

    ASSERT_EQ(loaded.size(), 2);
    EXPECT_EQ(loaded[0], entry);
}

TEST(CsvJournalRepositoryTest, NoFile) {
    CsvJournalRepository repository("no_such_file.csv");
    EXPECT_THROW(repository.load(), std::runtime_error);
}
