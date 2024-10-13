// my_numeric.h

#ifndef MY_NUMERIC_H
#define MY_NUMERIC_H

#include <string>

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
}

#endif // !MY_NUMERIC_H