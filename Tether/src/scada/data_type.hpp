#pragma once

#include <optional>
#include <string>

namespace Tether::SCADA {

    // Data types used in Modbus/SCADA address allocation
    enum class DataType {
        BOOL,
        INT,
        UINT,
        REAL,
        WORD,
        DWORD,
        UNKNOWN
    };

    // Convert string (e.g., "BOOL") to DataType
    std::optional<DataType> stringToDataType(const std::string& type);

    // Convert DataType to string representation
    std::optional<std::string> dataTypeToString(DataType type);
}