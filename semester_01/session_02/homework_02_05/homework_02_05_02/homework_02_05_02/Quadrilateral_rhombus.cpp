/* Quadrilateral_rhombus.cpp */

#include "Quadrilateral_rhombus.h"

namespace geometry {

	Quadrilateral_rhombus::Quadrilateral_rhombus(double side,
		double angle)
		: Quadrilateral_freeform({ side, side, side, side })
	{
		Quadrilateral_freeform::angles(
			{ angle, 180.0 - angle, angle, 180.0 - angle }
		);
	}

	Quadrilateral_rhombus::~Quadrilateral_rhombus()
	{
	}

	void Quadrilateral_rhombus::view_info() const
	{
		Quadrilateral_freeform::view_info();
	}
}