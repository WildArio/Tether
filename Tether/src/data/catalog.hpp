#pragma once

#include <vector>
#include "equipment.hpp"

namespace Tether::Data {
	std::vector<Breaker> loadBreakerCatalog();
	std::vector<Cable> loadCableCatalog();
	std::vector<Motor> loadMotorCatalog();
}
