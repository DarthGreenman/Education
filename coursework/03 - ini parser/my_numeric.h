// my_numeric.h

#ifndef MY_NUMERIC_H
#define MY_NUMERIC_H

#include "my_types.h"

#include <cmath>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace my 
{
	constexpr auto null = std::char_traits<char>::to_int_type('0');
	constexpr auto to_numeric(char ch) 
	{
		return std::char_traits<char>::to_int_type(ch) - null;
	}

	constexpr auto to_char(int numeric) 
	{
		return std::char_traits<char>::to_char_type(numeric + null);
	}


	template<typename Type,
		typename = std::enable_if_t<std::is_integral_v<Type> || std::is_floating_point_v<Type>>>
	static auto to_number(const std::string& string)
	{
		using namespace std;
		using namespace ascii;
		if (const auto pos = find_if_not(cbegin(string), cend(string),
			[](char ch) { return my::is_digit(ch) || ch == comma || ch == period; }); pos != cend(string))
		{
			throw invalid_argument{ "\nUnresolved symbol - value must be a numeric character in line: " +
			to_string(__LINE__) + ", file:\n" + std::string{ __FILE__ } + '\n' };
		}

		pair<size_t, size_t> number{}; // Структура { МАНТИССА, СТЕПЕНЬ }
		size_t number_bitness{};
		for (auto first = rbegin(string); first != rend(string); ++first)
		{
			if (*first == comma || *first == period)
				number.second = number_bitness;
			else
				number.first += my::to_numeric(*first) * static_cast<size_t>(pow(10, number_bitness++));
		}

		return static_cast<Type>(number.first) / static_cast<Type>(pow(10, number.second));
	}
}

#endif // !MY_NUMERIC_H