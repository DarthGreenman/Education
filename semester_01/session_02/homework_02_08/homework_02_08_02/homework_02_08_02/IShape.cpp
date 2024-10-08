/* IShape.cpp */

#include "IShape.h"
#include <cmath>
#include <numbers>
#include <numeric>

double geo::get_side(double adjacent_one, double adjacent_two,
	double angle_between)
{
	using std::pow; using std::sqrt; using std::cos;
	return sqrt(pow(adjacent_one, 2) +
		pow(adjacent_two, 2) -
		2 * adjacent_one * adjacent_two *
		cos(angle_between * std::numbers::pi / 180.0));
}

double geo::get_angle(const std::vector<double>& sides)
{
	const size_t size{ sides.size() };
	double angle{};
	switch (size) {
		case 3:
			using std::pow; using std::acos;
			angle = acos((pow(sides[0], 2) +
				pow(sides[1], 2) -
				pow(sides[2], 2)) / (2 * sides[0] * sides[1])) *
				180.0 / std::numbers::pi;
			break;
		case 4:
			angle = sides[0] * 180.0 * (sides.size() - 2) /
				std::accumulate(sides.cbegin(), sides.cend(), 0.0);
			break;
	}
	return angle;
}