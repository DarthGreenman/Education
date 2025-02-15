/* Triangle_rectangular.cpp */

#include "Triangle_rectangular.h"

namespace geometry {
	
	Triangle_rectangular::Triangle_rectangular(const Lines& sides)
		: Triangle_freeform(sides)
	{
	}

	Triangle_rectangular::~Triangle_rectangular()
	{
	}

	void Triangle_rectangular::view_info() const
	{
		Triangle_freeform::view_info();
	}
}