// my_numeric.h

#ifndef MY_NUMERIC_H
#define MY_NUMERIC_H

#include <cassert>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace my
{
	constexpr auto operator "" _z(unsigned long long number)
	{
		return static_cast<std::size_t>(number);
	}

	template<typename Type>
	struct numeric
	{
		template<typename Type>
		static auto odd_numbers(std::pair<Type, Type> number)
		{
			assert(number.first % 2 && number.second % 2 &"The numbers must be odd.");
			using namespace std;
			vector<Type> odd((number.second - number.first) / 2 + 1);
			for (auto first = begin(odd); first != end(odd); ++first, ++++number.first)
				*first = number.first;
			return odd;
		}

		template<typename Type>
		static auto is_prime(Type number)
		{
			for (Type div{ 3 }; div <= sqrt(number); ++div)
				if (!(number % div))
					return false;
			return true;
		}

		template<typename Type>
		static auto prime_numbers(Type number)
		{
			using namespace std;
			const auto odd =
				odd_numbers<Type>(make_pair(1, number % 2 ? number : number - 1));
			vector<Type> primers{};
			primers.reserve(size(odd));
			primers.push_back(2);
			primers.push_back(3);

			for (auto first = next(begin(odd), 2); first != end(odd); ++first)
				if (is_prime(*first))
					primers.push_back(*first);
			return primers;
		}

		template<typename Iter, typename = std::enable_if<
			std::is_same_v<typename std::iterator_traits<Iter>::iterator_category, std::input_iterator_tag>&&
			std::is_integral_v<typename std::iterator_traits<Iter>::value_type>>>
		static auto polynomial(typename std::iterator_traits<Iter>::value_type number, Iter first, Iter last)
		{
			using namespace std;
			vector<typename std::iterator_traits<Iter>::value_type> polynomile{};
			polynomile.reserve(number);
			for (; number != 1; ++first)
				for (; !(number % *first); number /= *first)
					polynomile.push_back(*first);
			return polynomile;
		}
	};
}

#endif // !MY_NUMERIC_H