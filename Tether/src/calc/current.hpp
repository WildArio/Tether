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

	double current(double power, double voltage, double cosPhi, double eff, LoadType loadType);

	namespace {
		template <typename T>
		std::optional<double> estimateCurrentByPower(const std::vector<T>& catalog, double targetPower) {
			assert(!catalog.empty() && "Catalog is empty, can't estimateCurrentByPower!");

			auto item{ std::lower_bound(catalog.begin(), catalog.end(), targetPower,
				[](const T& m, double p) {
					return m.power < p;
			}) }; // TODO MOVE TO catalog 
			//assert(false && "MOVE LOWERBOUND IN estimateCurrentByPower");

			if (item == catalog.begin()) {
				return item->nominalCurrent;
			}

			if (item == catalog.end()) {
				return catalog.back().nominalCurrent;
			}

			const auto& motor1{ *(item - 1) };
			const auto& motor2{ *(item) };

			if (motor1.power >= motor2.power) {
				return std::nullopt;
			}

			return Tether::Math::interpolate(targetPower, motor1.power, motor1.nominalCurrent, motor2.power, motor2.nominalCurrent);
		}
	}

	template <typename T, typename U>
	std::optional<double> currentByContext(const std::vector<T>& catalog, const U& ctx) {
		assert(!catalog.empty() && "Catalog is empty, can't findCurrentByContext");

		if (ctx.selectFromCatalog) {
			// We can use assert, even if it's a user input. We should check user input BEFORE calling "API";
			assert(ctx.power.has_value() && ".power value was null in findCurrentByContext call");

			if (ctx.loadType != LoadType::THREE_PHASE) {
				return std::nullopt;
			}

			if (ctx.power.value() < 0.0) {
				return std::nullopt;
			}
			else if (ctx.power.value() < 1e-9) {
				return 0.0;
			}


			return estimateCurrentByPower(catalog, ctx.power.value()); // should return std::optional<double> and nullopt
		}
		else {
			assert(ctx.power.has_value() && ctx.voltage.has_value() && ctx.cosPhi.has_value() && ctx.eff.has_value() &&
				"Some context is null in findCurrentByContext call");

			double power{ ctx.power.value() };
			double voltage{ ctx.voltage.value() };
			double cosPhi{ ctx.cosPhi.value() };
			double eff{ ctx.eff.value() };

			if (power >= 0 && voltage > 0 && cosPhi > 0 && cosPhi <= 1.0 && eff > 0.0 && eff <= 1.0) {
				return Tether::Calc::current(power, voltage, cosPhi, eff, ctx.loadType);
			}
			else {
				return std::nullopt;
			}
		}
	}
}
