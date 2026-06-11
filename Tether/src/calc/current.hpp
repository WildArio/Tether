#pragma once
#include <algorithm>
#include <cassert>
#include <optional>
#include <vector>
#include "consts.hpp"
#include "core/math_utils.hpp"
#include "data/equipment.hpp"
#include "data/catalog.hpp"

namespace Tether::Calc{

	enum class LoadType { THREE_PHASE, ONE_PHASE };

	struct CalculateCurrentContext {
		std::optional<double> power;
		std::optional<double> voltage;
		std::optional<double> cosPhi;
		std::optional<double> eff;
		bool selectFromCatalog;
		LoadType loadType;
	};

	namespace {
		std::optional<double> estimateCurrentByPower(const std::vector<Tether::Data::Motor>& catalog, double targetPower);
	}

	double current(double power, double voltage, double cosPhi, double eff, LoadType loadType);

	std::optional<double> currentByContext(const std::vector<Tether::Data::Motor>& catalog, const CalculateCurrentContext& ctx);
}
