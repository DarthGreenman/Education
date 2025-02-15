// my_numeric.h

#ifndef MY_NUMERIC_H
#define MY_NUMERIC_H

#include <string_view>
#include <type_traits>

namespace numbers {
	namespace helper {
		using size_type = typename std::string_view::size_type;
		using const_iterator = typename std::string_view::const_iterator;
		
		constexpr auto length_impl(std::string_view Value, size_type Length,
			const_iterator Begin, const_iterator End) -> decltype(Length) 
		{
			return Begin == End ? Length :
				length_impl(Value, ++Length, ++Begin, End);
		}
	}

	constexpr auto null = std::char_traits<char>::to_int_type('0');
	constexpr auto to_numeric(char Ch) {
		return std::char_traits<char>::to_int_type(Ch) - null;
	}

	constexpr auto to_char(int Numeric) {
		return std::char_traits<char>::to_char_type(Numeric + null);
	}

	constexpr auto length(std::string_view Value) {
		return helper::length_impl(Value, 0, 
			std::cbegin(Value), std::cend(Value));
	}
}
#endif // !MY_NUMERIC_H