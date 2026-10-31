#pragma once

#include <vector>

#include "application/JournalEntry.hpp"

namespace casino {

class IJournalRepository {
public:
    virtual ~IJournalRepository() = default;

    virtual void save(const std::vector<JournalEntry>& entries) = 0;
    virtual std::vector<JournalEntry> load() = 0;
};

}
