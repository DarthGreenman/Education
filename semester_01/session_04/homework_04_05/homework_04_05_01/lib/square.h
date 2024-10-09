/* square.h */

#ifndef SQUARE_H
#define SQUARE_H

#include <vector>
#include <algorithm>
#include <concepts>

namespace seq
{
	/* Концепт формирующий список возможных типов в шаблонах */
	template<typename T>
	concept Appropriate_type = std::same_as<T, int> || std::same_as<T, double>;

	template<typename T>
		requires Appropriate_type<T>
	T square(T Value)
	{
		/* Возвращает значение без учета переполнения значения типа */
		return Value * Value;
	}

	template<typename T>
	std::vector<T> square(const std::vector<T>& Value)
	{
		/* Возвращает значение без учета переполнения значения типа */
		std::vector<T> result(Value.size());
		std::transform(Value.cbegin(), Value.cend(), 
			result.begin(), [](T Number) { return Number * Number; });
		return result;
	}
}

#endif /* SQUARE_H */