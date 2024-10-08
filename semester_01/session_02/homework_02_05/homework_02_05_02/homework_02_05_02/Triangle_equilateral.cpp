/* Triangle_equilateral.cpp */

#include "Triangle_equilateral.h"

namespace geometry {

	Triangle_equilateral::Triangle_equilateral(double side)
		: Triangle_freeform({ side, side, side })
	{
	}

	Triangle_equilateral::~Triangle_equilateral()
	{
	}

	void Triangle_equilateral::view_info() const
	{
		Triangle_freeform::view_info();
	}
}