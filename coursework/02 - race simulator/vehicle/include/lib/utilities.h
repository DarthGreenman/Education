/* utilities.h */

#ifndef RACE_SIMULATOR_UTILITIES_H
#define RACE_SIMULATOR_UTILITIES_H

#include <limits>
#include <cmath>
#include <concepts>

namespace bag
{
	/* Шаблоны функций для сравнения типов size_t, double. */
	template<typename T>
	concept Type_comparison = std::same_as<T, size_t> || std::same_as<T, double>;

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
#endif /* RACE_SIMULATOR_UTILITIES_H */