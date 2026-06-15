#include <cassert>
#include "current.hpp"
#include "consts.hpp"

namespace Tether::Calc {

	namespace {
		std::optional<double> estimateCurrentByPower(const std::vector<Tether::Data::Motor>& catalog, double targetPower) {
			assert(!catalog.empty() && "Catalog is empty, can't estimateCurrentByPower!");

			auto item{ std::lower_bound(catalog.begin(), catalog.end(), targetPower,
				[](const Tether::Data::Motor& m, double p) {
					return m.power < p;
			}) };

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

	double current(double power, double voltage, double cosPhi, double eff, LoadType loadType) {
		assert(power >= 0.0 && voltage > 0.0 && eff > 0.0 && eff <= 1.0 && cosPhi > 0.0 && cosPhi <= 1.0);

		double coefficient{ loadType == LoadType::THREE_PHASE ? Tether::Constants::SQRT3 : 1.0 };

		return power / (voltage * eff * coefficient * cosPhi);
	}

	std::optional<double> currentByContext(const std::vector<Tether::Data::Motor>& catalog, const CalculateCurrentContext& ctx) {
		assert(!catalog.empty() && "Catalog is empty, can't findCurrentByContext");

		if (ctx.selectFromCatalog) {
			// We can use assert, even if it's a user input. We should check user input BEFORE calling "API";
			assert(ctx.power.has_value() && ".power value was null in findCurrentByContext call");

			double power{ ctx.power.value() };

			if (ctx.loadType != LoadType::THREE_PHASE) {
				return std::nullopt;
			}

			if (power < 0.0) {
				return std::nullopt;
			}
			else if (power < 1e-9) {
				return 0.0;
			}

			return estimateCurrentByPower(catalog, power); // should return std::optional<double> and nullopt
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
