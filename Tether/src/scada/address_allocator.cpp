#include <cassert>
#include <optional>
#include "address_allocator.hpp"

namespace Tether::SCADA {

    // We should hide internal usage in anonymouse namespace
    namespace {

        // Internal counters
        std::uint16_t boolOffset = 0;
        std::uint16_t intOffset = 0;
        std::uint16_t uintOffset = 0;
        std::uint16_t realOffset = 0;
        std::uint16_t wordOffset = 0;
        std::uint16_t dwordOffset = 0;

        // Base addresses
        constexpr std::uint16_t BASE_BOOL = 15000;
        constexpr std::uint16_t BASE_INT = 20000;
        constexpr std::uint16_t BASE_UINT = 25000;
        constexpr std::uint16_t BASE_REAL = 40000;
        constexpr std::uint16_t BASE_WORD = 30000;
        constexpr std::uint16_t BASE_DWORD = 35000;
    }

    std::optional<std::uint16_t> allocateAddress(DataType type) {
        switch (type) {
        case DataType::BOOL:
            return BASE_BOOL + boolOffset++;
        case DataType::INT:
            return BASE_INT + intOffset++;
        case DataType::UINT:
            return BASE_UINT + uintOffset++;
        case DataType::REAL: {
            std::uint16_t addr = BASE_REAL + realOffset;
            realOffset += 2;           // REAL 2 registerrs
            return addr;
        }
        case DataType::WORD:
            return BASE_WORD + wordOffset++;
        case DataType::DWORD: {
            std::uint16_t addr = BASE_DWORD + dwordOffset;
            dwordOffset += 2;          // DWORD 2 registers
            return addr;
        }
        default:
            return std::nullopt;
        }
    }
}