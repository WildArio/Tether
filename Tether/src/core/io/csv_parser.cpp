// Tether/src/scada/csv_parser.cpp

#include <sstream>
#include <stdexcept>
#include "csv_parser.hpp"

namespace Tether::IO {

    std::ifstream openCsv(const std::string& path) {
        std::ifstream file(path);

        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + path);
        }

        return file; // move semantics, NRVO
    }

    std::vector<std::vector<std::string>> parseCsv(std::ifstream& file) {
        std::vector<std::vector<std::string>> rows;
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string field;

            while (std::getline(ss, field, ',')) {
                row.push_back(field);
            }

            rows.push_back(std::move(row));
        }
        return rows;
    }
}