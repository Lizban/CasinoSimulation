#include "infrastructure/CsvJournalRepository.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace casino {

namespace {

const std::string kHeader =
    "spin,bet_type,target,amount,pocket,pocket_color,payout,player_balance,cashbox_balance";
const std::size_t kColumnCount = 9;

void removeCarriageReturn(std::string& line) {
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
}

std::vector<std::string> splitByComma(const std::string& line) {
    std::vector<std::string> fields;
    std::istringstream stream(line);
    std::string field;
    while (std::getline(stream, field, ',')) {
        fields.push_back(field);
    }
    return fields;
}

JournalEntry parseLine(const std::string& line, int lineNumber) {
    const std::string error = "Журнал повреждён, строка " + std::to_string(lineNumber);
    std::vector<std::string> fields = splitByComma(line);
    if (fields.size() != kColumnCount) {
        throw std::runtime_error(error);
    }
    try {
        JournalEntry entry;
        entry.spin = std::stoi(fields[0]);
        entry.betType = fields[1];
        entry.target = fields[2];
        entry.amount = std::stoll(fields[3]);
        entry.pocket = std::stoi(fields[4]);
        entry.pocketColor = fields[5];
        entry.payout = std::stoll(fields[6]);
        entry.playerBalance = std::stoll(fields[7]);
        entry.cashboxBalance = std::stoll(fields[8]);
        return entry;
    } catch (const std::logic_error&) {
        throw std::runtime_error(error);
    }
}

}

CsvJournalRepository::CsvJournalRepository(std::string path) : path_(std::move(path)) {}

void CsvJournalRepository::save(const std::vector<JournalEntry>& entries) {
    std::ofstream file(path_);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + path_);
    }
    file << kHeader << '\n';
    for (const JournalEntry& e : entries) {
        file << e.spin << ',' << e.betType << ',' << e.target << ',' << e.amount << ','
             << e.pocket << ',' << e.pocketColor << ',' << e.payout << ',' << e.playerBalance
             << ',' << e.cashboxBalance << '\n';
    }
}

std::vector<JournalEntry> CsvJournalRepository::load() {
    std::ifstream file(path_);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл журнала: " + path_);
    }

    std::string line;
    std::getline(file, line);
    removeCarriageReturn(line);
    if (line != kHeader) {
        throw std::runtime_error("Файл не похож на журнал рулетки: " + path_);
    }

    std::vector<JournalEntry> entries;
    int lineNumber = 1;
    while (std::getline(file, line)) {
        ++lineNumber;
        removeCarriageReturn(line);
        if (line.empty()) {
            continue;
        }
        entries.push_back(parseLine(line, lineNumber));
    }
    return entries;
}

}
