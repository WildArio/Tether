#include <cassert>
#include "math_utils.hpp"

namespace Tether::Math {
	double interpolate(double x, double x1, double y1, double x2, double y2) {
		assert(x1 < x2 && "Interpolation points are equal or x2 less than x1!");

		return y1 + ((x - x1) / (x2 - x1) * (y2 - y1));
	}

}