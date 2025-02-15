/* shape.cpp */

#include "shape.h"

namespace geometry 
{
	shape::shape()
	{
	}

	shape::~shape()
	{
	}

	unsigned shape::number_sides() const
	{
		return number_sides_;
	}
}