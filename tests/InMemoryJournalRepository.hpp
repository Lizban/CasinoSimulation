#pragma once

#include <vector>

#include "application/IJournalRepository.hpp"

namespace casino {

class InMemoryJournalRepository : public IJournalRepository {
public:
    void save(const std::vector<JournalEntry>& entries) override { saved = entries; }
    std::vector<JournalEntry> load() override { return saved; }

    std::vector<JournalEntry> saved;
};

}
