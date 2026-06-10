#include <cassert>
#include "current.hpp"
#include "consts.hpp"

namespace Tether::Calc {
	double current(double power, double voltage, double cosPhi, double eff, LoadType loadType) {
		assert(power >= 0.0 && voltage > 0.0 && eff > 0.0 && eff <= 1.0 && cosPhi > 0.0 && cosPhi <= 1.0);

		double coefficient{ loadType == LoadType::THREE_PHASE ? SQRT3 : 1.0 };

		return power / (voltage * eff * coefficient * cosPhi);
	}
}

