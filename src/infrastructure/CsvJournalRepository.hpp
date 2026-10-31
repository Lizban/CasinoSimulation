#pragma once

#include <string>
#include <vector>

#include "application/IJournalRepository.hpp"

namespace casino {

class CsvJournalRepository : public IJournalRepository {
public:
    explicit CsvJournalRepository(std::string path);

    void save(const std::vector<JournalEntry>& entries) override;
    std::vector<JournalEntry> load() override;

private:
    std::string path_;
};

}
