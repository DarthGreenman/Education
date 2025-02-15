/* Quadrilateral_square.cpp */

#include "Quadrilateral_square.h"

namespace geometry {

	Quadrilateral_square::Quadrilateral_square(double side)
		: Quadrilateral_freeform({ side, side, side, side })
	{
		Quadrilateral_freeform::angles(
			{ 90.0, 90.0, 90.0, 90.0 }
		);
	}

	Quadrilateral_square::~Quadrilateral_square()
	{
	}

	void Quadrilateral_square::view_info() const
	{
		Quadrilateral_freeform::view_info();
	}
}