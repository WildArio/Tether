#include <vector>
#include "equipment.hpp"
#include "selection.hpp"

static constexpr double BREAKER_COEF{ 1.2 };

namespace Tether::Data {

	std::optional<Breaker> selectBreaker(const std::vector<Breaker>& catalog, double current) {
		return findNearest(catalog, current,
			[](const Breaker& b, double val) { return b.nominalCurrent < val; });
	}

	std::optional<Cable> selectCable(const std::vector<Cable>& catalog, double current) {
		return findNearest(catalog, current,
			[](const Cable& c, double val) { 
				double realCurrent{ c.maxCurrent * BREAKER_COEF };
				return realCurrent < val;
			}
		);
	}

	std::optional<Motor> selectMotor(const std::vector<Motor>& catalog, double power) {
		return findNearest(catalog, power,
			[](const Motor& m, double val) { return m.power < val; });
	}
}
