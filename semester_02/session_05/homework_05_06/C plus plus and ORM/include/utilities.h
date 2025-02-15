// utilities.h

#ifndef UTILITIES_H
#define UTILITIES_H


#include <__msvc_string_view.hpp>
#include <algorithm>
#include <cmath>
#include <iosfwd>
#include <iterator>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace utls
{
	namespace helper
	{
		template<typename Char_type>
		inline auto offset()
		{
			return std::char_traits<Char_type>::to_int_type('0');
		}
	}

	template<typename Char_type>
	inline auto to_numeric(Char_type ch)
	{
		return std::char_traits<Char_type>::to_int_type(ch) - helper::offset<Char_type>();
	}

	inline auto to_number(const std::string& number_as_string)
	{
		using namespace std;
		struct number_view
		{
			void operator()(const typename iterator_traits<decay_t<decltype(rbegin(number_as_string))>>::value_type& elem)
			{
				const decltype(value.first) numeric = to_numeric(elem);
				value.first += numeric * static_cast<decltype(value.first)>(std::pow(10, value.second++));
			}
			pair<size_t, size_t> value = make_pair(0ull, 0ull);
		};

		const auto number = for_each(rbegin(number_as_string), rend(number_as_string), number_view{});
		return number.value.first;
	}

	inline auto split(std::istringstream& stream, char delim)
	{
		std::vector<std::string> parts{};
		parts.reserve(64ull);
		for (std::string part{}; std::getline(stream, part, delim);)
			parts.emplace_back(std::move(part));
		return parts;
	}
}

#endif // !UTILITIES_H

