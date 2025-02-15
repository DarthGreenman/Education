/* utilities_example_01.cpp */

#include "utilities_example_01.h"

namespace seq
{
	template int min(int lhs, int rhs);
	template double min(double lhs, double rhs);

	template<typename T> T min(T lhs, T rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}
}
