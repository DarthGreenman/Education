// my_hash.h

#ifndef MY_HASH_H
#define MY_HASH_H

#include <cmath>
#include <iterator>
#include <numeric>
#include <string>
#include <type_traits>
#include <utility>

namespace my
{	
	struct hash
	{
		template<typename Iter, typename = std::enable_if<
			std::is_same_v<typename std::iterator_traits<Iter>::iterator_category, std::input_iterator_tag>&&
			std::is_integral_v<typename std::iterator_traits<Iter>::value_type>>>
		static constexpr auto generate(Iter first, Iter last)
		{
			return std::accumulate(first, last, 0ull);
		}

		constexpr auto generate(std::string message)
		{
			return generate(std::cbegin(message), std::cend(message));
		}

		template<typename Iter, typename = std::enable_if<
			std::is_same_v<typename std::iterator_traits<Iter>::iterator_category, std::input_iterator_tag>&&
			std::is_integral_v<typename std::iterator_traits<Iter>::value_type>>>
		static auto generate(Iter first, Iter last, std::pair<std::size_t, std::size_t> param)
		{
			using namespace std;
			size_t count{};
			auto algorithm = [param, &count](size_t init, size_t elem) {
				return init + elem * static_cast<size_t>(std::pow(param.first, count++));
				};
			return std::accumulate(first, last, 0ull, algorithm) % param.second;
		}

	};
}

#endif // !MY_HASH_H