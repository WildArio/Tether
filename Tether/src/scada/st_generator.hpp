#pragma once

#include <optional>
#include <string>
#include <vector>
#include "variable.hpp"

namespace Tether::SCADA {

	// Generate ST (Structured Text) code lines for given variable.
	// Returns vector of lines (one for standard types, two for REAL/DWORD).
	std::optional<std::vector<std::string>> generateStLines(const Variable& variable);
}