// my_ultralong.h

#ifndef MY_ULTRALONG_H
#define MY_ULTRALONG_H

#include "my_numeric.h"
#include "my_utilities.h"

#include <algorithm>
#include <bitset>
#include <cctype>
#include <cstddef>
#include <exception>
#include <initializer_list>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace my
{
	auto error_message(const std::string& message, int line)
	{
		return std::string{ message + std::to_string(line) + ", file:\n" + std::string{ __FILE__ } + '\n' };
	}

	template<std::size_t Width_in_bits>
	class ultralong
	{
		static_assert(!(Width_in_bits % 8ull), "The bit field width must be a multiple of 8.");
	public:
		using size_type = std::size_t;
		using bitset = std::bitset<Width_in_bits>;

		template<size_type Width_in_bits> friend
		std::ostream& operator<<(std::ostream& Os, const ultralong<Width_in_bits>& number);
		
		constexpr ultralong() = default;
		explicit ultralong(const std::string& number_a_string) : 
			ultralong(std::crbegin(number_a_string), std::crend(number_a_string))
		{}
		
		template<typename Type, typename = std::enable_if_t<std::is_integral_v<Type>>>
		ultralong(const std::vector<Type>& number_a_vector) :
			ultralong(std::crbegin(number_a_vector), std::crend(number_a_vector))
		{}

		template<typename Type, typename = std::enable_if_t<std::is_integral_v<Type>>>
		ultralong(std::initializer_list<Type> number_a_list) :
			ultralong(std::crbegin(number_a_list), std::crend(number_a_list))
		{}
		
		template<typename Iterator>
		ultralong(Iterator first, Iterator last) : ultralong()
		{
			try {
				check(first, last);
			}
			catch (const std::exception& err) {
				throw err;
			}
			const auto number = construct(first, last);
			bits::copy(number, number_);
		}

		ultralong(const ultralong&) = delete;
		ultralong(ultralong&&) = delete;

		~ultralong() = default;

		ultralong& operator=(const ultralong&) noexcept = delete;
		ultralong& operator=(ultralong&&) noexcept = delete;

	private:
		template<typename Iterator>
		auto check(Iterator first, Iterator last)
		{
			using namespace std;
			auto is_digit = [](typename iterator_traits<Iterator>::value_type elem) 
				{
					return is_same_v<decltype(elem), char> ? isdigit(elem) : true; 
				};

			if (find_if_not(first, last, is_digit) != last)
				throw invalid_argument{ error_message("A character cannot be converted to a digit.", __LINE__) };
			
			if (Width_in_bits / 8ull < distance(first, last))
				throw out_of_range{ error_message("The type does not have the ability to accommodate the number.", __LINE__) };
		}

		// Конструирует число.
		template<typename Iterator>
		auto construct(Iterator first, Iterator last)
		{
			using namespace std;
			struct number_representation
			{
				void operator()(const typename iterator_traits<Iterator>::value_type& elem)
				{
					const bitset numeric{ static_cast<size_type>(is_same_v<decay_t<decltype(elem)>, char > ?
						my::to_numeric(elem) : elem) };
					const auto shift = representation.second++ * 8ull;
					representation.first ^= numeric << shift;
				}
				pair<bitset, size_type> representation{}; // {{ big-endian },  {Индекс самого младшего байта }}
			};
			const auto number = for_each(first, last, number_representation());
			return number.representation.first;
		}


	private:
		bitset number_{};
	};

	template<std::size_t Width_in_bits>
	std::ostream& operator<<(std::ostream& os, const ultralong<Width_in_bits>& number) {
		using size_type = typename ultralong<Width_in_bits>::size_type;
		for (size_type pos{}; pos < Width_in_bits; ++pos) 
			os << number.number_.test(Width_in_bits - pos - 1ull);
		return os;
	}
}

#endif // !MY_ULTRALONG_H