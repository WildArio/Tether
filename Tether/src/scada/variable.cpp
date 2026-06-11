#include <sstream>
#include <stdexcept>
#include <vector>
#include "address_allocator.hpp"
#include "data_type.hpp"
#include "variable.hpp"

namespace Tether::SCADA {

    std::vector<Variable> buildVariablesFromCsv(const std::vector<std::vector<std::string>>& rows) {
        if (rows.empty()) {
            throw std::runtime_error("Empty CSV input!");
        }

        std::vector<Variable> vars;
        // Determine if first row is a header
        size_t startRow = (rows[0][0] == "Name") ? 1 : 0;

        for (size_t i = startRow; i < rows.size(); ++i) {
            const auto& row = rows[i];

            if (row.size() < 2 || row[0].empty()) {
                throw std::runtime_error("Invalid row format at line: " + std::to_string(i));
            }

            Variable v;
            v.name = row[0];
            auto dtOpt = stringToDataType(row[1]);

            if (!dtOpt.has_value()) {
                throw std::runtime_error("Unknown data type at line " + std::to_string(i) + ": " + row[1]);
            }

            v.dataType = dtOpt.value();
            auto addrOpt = Tether::SCADA::allocateAddress(v.dataType);

            if (!addrOpt.has_value()) {
                throw std::runtime_error("Failed to allocate address for " + v.name);
            }

            v.modbusAddress = addrOpt.value();
            v.description = (row.size() > 2) ? row[2] : "";
            vars.push_back(std::move(v));
        }
        return vars;
    }
}