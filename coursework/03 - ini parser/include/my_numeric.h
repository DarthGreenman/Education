// my_numeric.h

#ifndef MY_NUMERIC_H
#define MY_NUMERIC_H

#include "my_types.h"

#include <cmath>
#include <string>
#include <type_traits>

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
		auto digit = string.length() - Type{ 1 };
		Type number{};
		for (auto first = begin(string); first != end(string); ++first, --digit)
		{
			const auto numeric = to_numeric(*first);
			number += numeric * static_cast<Type>(pow(10, digit));
		}
		return number;
	}
}
#endif // !MY_NUMERIC_H