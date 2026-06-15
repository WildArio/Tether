#include <optional>
#include <string>
#include <vector>
#include "st_generator.hpp"
#include "variable.hpp"

namespace Tether::SCADA {

    std::optional<std::vector<std::string>> generateStLines(const Variable& variable) {
        std::vector<std::string> lines;
        lines.reserve(2);

        switch (variable.dataType) {
        case DataType::BOOL:
        case DataType::INT:
        case DataType::UINT:
        case DataType::WORD: {
            std::string line = "RegHoldingBuf[" +
                std::to_string(variable.modbusAddress) +
                "] := TO_WORD(" + variable.name + ");";
            lines.push_back(line);
            break;
        }

        case DataType::REAL:
        case DataType::DWORD: {
            // TODO: Split into high and low word (e.g., using WORD_HI and WORD_LO functions)
            std::string firstLine = "RegHoldingBuf[" +
                std::to_string(variable.modbusAddress) +
                "] := TO_WORD(" + variable.name + ");";
            std::string secondLine = "RegHoldingBuf[" +
                std::to_string(variable.modbusAddress + 1U) +
                "] := TO_WORD(" + variable.name + ");";
            lines.push_back(firstLine);
            lines.push_back(secondLine);
            break;
        }

        default:
            // Should never happen if enum is used correctly
            return std::nullopt;
        }

        return lines;
    }
}