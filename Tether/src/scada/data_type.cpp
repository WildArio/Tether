#include <cassert>
#include <optional>
#include <string>
#include "data_type.hpp"

namespace Tether::SCADA {

    std::optional<DataType> stringToDataType(const std::string& type) {
        if (type == "BOOL") return DataType::BOOL;
        if (type == "INT")  return DataType::INT;
        if (type == "UINT") return DataType::UINT;
        if (type == "REAL") return DataType::REAL;
        if (type == "WORD") return DataType::WORD;
        if (type == "DWORD") return DataType::DWORD;

        // Should never happen if enum is used correctly
        return std::nullopt;
    }

    std::optional<std::string> dataTypeToString(DataType type) {
        switch (type) {
        case DataType::BOOL:  return "BOOL";
        case DataType::INT:   return "INT";
        case DataType::UINT:  return "UINT";
        case DataType::REAL:  return "REAL";
        case DataType::WORD:  return "WORD";
        case DataType::DWORD: return "DWORD";
        default:
            // Should never happen if enum is used correctly
            return std::nullopt;
        }
    }

}