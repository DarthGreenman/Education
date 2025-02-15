// my_utilities.h

#ifndef MY_LIBRARY_UTILITIES_H
#define MY_LIBRARY_UTILITIES_H

#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>

namespace seq
{
	/* Шаблоны функций для сравнения типов double. */
	template<typename T>
	concept Type_comparison = std::same_as<T, double>;

	template<typename Arg,
		double eps = std::numeric_limits<double>::epsilon()>
		requires Type_comparison<Arg>
	bool equal(Arg lhs, Arg rhs)
	{
		return std::fabs(lhs - rhs) <= eps;
	}

	template<typename Arg,
		double eps = std::numeric_limits<double>::epsilon()>
		requires Type_comparison<Arg>
	bool less(Arg lhs, Arg rhs)
	{
		return lhs < (rhs - eps);
	}

	template<typename Arg,
		double eps = std::numeric_limits<double>::epsilon()>
		requires Type_comparison<Arg>
	bool greater(Arg lhs, Arg rhs)
	{
		return lhs > (rhs + eps);
	}
}

#endif // MY_LIBRARY_UTILITIES_H