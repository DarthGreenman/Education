// my_hash.h

#ifndef MY_HASH_H
#define MY_HASH_H

#include <iterator>
#include <numeric>
#include <type_traits>
#include <string_view>

namespace my
{
	constexpr auto operator "" _z(unsigned long long number)
	{
		return static_cast<std::size_t>(number);
	}

	template<typename Iter,
		typename = std::enable_if<
		std::is_same_v<typename std::iterator_traits<Iter>::iterator_category, std::input_iterator_tag>&&
		std::is_integral_v<typename std::iterator_traits<Iter>::value_type>>>
	constexpr auto hash_function(Iter first, Iter last)
	{
		return std::accumulate(first, last, 0_z);
	}

	constexpr auto hash_function(std::string_view message)
	{
		return hash_function(std::cbegin(message), std::cend(message));
	}
}

#endif // !MY_HASH_H