/* Triangle_isosceles.cpp */

#include "Triangle_isosceles.h"

namespace geometry {

	Triangle_isosceles::Triangle_isosceles(double side_one,
		double side_third)
		: Triangle_freeform({ side_one, side_one, side_third })
	{
	}

	Triangle_isosceles::~Triangle_isosceles()
	{
	}

	void Triangle_isosceles::view_info() const
	{
		Triangle_freeform::view_info();
	}
}