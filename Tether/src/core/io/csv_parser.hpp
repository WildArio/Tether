// Tether/src/scada/csv_parser.hpp
#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "scada/variable.hpp"

namespace Tether::IO {

	// Open CSV file for reading
	std::ifstream openCsv(const std::string& path);

	// Parse already opened CSV stream into rows of strings
	std::vector<std::vector<std::string>> parseCsv(std::ifstream& file);
}