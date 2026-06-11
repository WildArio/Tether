#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "data_type.hpp"

namespace Tether::SCADA {

    struct Variable {
        std::string name;
        DataType dataType;
        std::uint16_t modbusAddress;
        std::string description;
    };

    // Build list of Variable structures from parsed CSV rows.
    // Assumes optional header row with "Name", "Type", "Description".
    // Throws std::runtime_error on invalid input.
    std::vector<Variable> buildVariablesFromCsv(const std::vector<std::vector<std::string>>& rows);
}