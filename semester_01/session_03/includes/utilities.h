/* utilities.h */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <limits>
#include <concepts>
#include <type_traits>
#include <cmath>
#include <utility>

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

	enum class Method : size_t {
		by_default = 50, netology = 30
	};

	template<Method M = Method::by_default>
	struct Size_control
	{
		static std::pair<size_t, size_t> construct(size_t Size)
		{
			switch (M)
			{
			case seq::Method::by_default:
				return std::make_pair(Size, Size / 2);
			case seq::Method::netology:
				return std::make_pair(Size, Size / 2);
			}
		}
		static std::pair<size_t, size_t> scale(size_t Size, size_t New_size);
	};
}

#endif /* UTILITIES_H */