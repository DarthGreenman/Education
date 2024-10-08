/* Quadrilateral_parallelogram.cpp */

#include "Quadrilateral_parallelogram.h"

namespace geometry {

	Quadrilateral_parallelogram::Quadrilateral_parallelogram(double first_side,
		double second_side, double angle)
		: Quadrilateral_freeform({ first_side, second_side,
			first_side, second_side })
	{
		Quadrilateral_freeform::angles(
			{ angle, 180.0 - angle, angle, 180.0 - angle }
		);
	}

	Quadrilateral_parallelogram::~Quadrilateral_parallelogram()
	{
	}

	void Quadrilateral_parallelogram::view_info() const
	{
		Quadrilateral_freeform::view_info();
	}
}