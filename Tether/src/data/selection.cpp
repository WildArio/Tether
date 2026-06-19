#include <vector>
#include "equipment.hpp"
#include "selection.hpp"

namespace Tether::Data {

	std::optional<Breaker> selectBreaker(const std::vector<Breaker>& catalog, double current, double heatCoef) {
		if (catalog.empty()) return std::nullopt;

		auto it{ Detail::findFirstGreaterOrEqual(catalog, current * heatCoef, [](const Breaker& b) { return b.nominalCurrent; }) };

		return (it != catalog.end()) ? *it : *std::prev(it);
	}

	std::optional<Cable> selectCable(const std::vector<Cable>& catalog, double current) {
		if (catalog.empty()) return std::nullopt;

		auto it{ Detail::findFirstGreaterOrEqual(catalog, current, [](const Cable& c) { return c.maxCurrent; }) };

		return (it != catalog.end()) ? *it : *std::prev(it);
	}

	std::optional<Motor> selectMotor(const std::vector<Motor>& catalog, double power) {
		if (catalog.empty()) return std::nullopt;

		return Detail::findNearest(catalog, power, [](const Motor& m) { return m.power; });
	}
}
