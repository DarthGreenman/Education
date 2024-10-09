/* utilities_example_02.cpp */

#include "utilities_example_02.h"

namespace seq
{
	template<typename T> T max(T lhs, T rhs)
	{
		return lhs > rhs ? lhs : rhs;
	}
}