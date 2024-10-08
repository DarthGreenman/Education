// my_hash.h

#ifndef MY_HASH_H
#define MY_HASH_H

#include <__msvc_string_view.hpp>
#include <cmath>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <utility>


namespace my
{
	constexpr auto operator "" _z(unsigned long long number)
	{
		return static_cast<std::size_t>(number);
	}

	auto is_prime(std::size_t n) {
		for (std::size_t i{ 2 }; i <= sqrt(n); ++i)
			if (n % i == 0)
				return false;
		return true;
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

	template<typename Iter,
		typename = std::enable_if<
		std::is_same_v<typename std::iterator_traits<Iter>::iterator_category, std::input_iterator_tag>&&
		std::is_integral_v<typename std::iterator_traits<Iter>::value_type>>>
	auto hash_function(Iter first, Iter last, std::pair<std::size_t, std::size_t> param)
	{
		using namespace std;
		size_t count{};
		auto algorithm = [param, &count](size_t init, size_t elem) {
			return init + elem * std::pow(param.first, count++);
		};
		return std::accumulate(first, last, 0_z, algorithm) % param.second;
	}
}

#endif // !MY_HASH_H