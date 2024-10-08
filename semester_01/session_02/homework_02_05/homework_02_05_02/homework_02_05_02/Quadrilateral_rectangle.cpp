/* Quadrilateral_rectangle.cpp */

#include "Quadrilateral_rectangle.h"

namespace geometry {

	Quadrilateral_rectangle::Quadrilateral_rectangle(double side_one,
		double side_two)
		: Quadrilateral_freeform( {side_one, side_two,
			side_one, side_two })
	{
		Quadrilateral_freeform::angles(
			{ 90.0, 90.0, 90.0, 90.0 }
		);
	}

	Quadrilateral_rectangle::~Quadrilateral_rectangle()
	{
	}

	void Quadrilateral_rectangle::view_info() const
	{
		Quadrilateral_freeform::view_info();
	}
}