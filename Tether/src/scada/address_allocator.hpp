#pragma once

#include <cstdint>
#include "data_type.hpp"

namespace Tether::SCADA {

	// Allocate next free address for given data type.
	// Uses global internal counters (shared across all calls).
	std::optional<std::uint16_t> allocateAddress(DataType type);

}