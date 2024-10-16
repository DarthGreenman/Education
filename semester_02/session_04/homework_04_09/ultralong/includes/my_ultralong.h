// my_ultralong.h

#ifndef MY_ULTRALONG_H
#define MY_ULTRALONG_H

// Десятичные числа — специальный вид представления числовой информации,
// в основу которого положен принцип кодирования каждой десятичной цифры числа
// группой из четырех бит. При этом каждый байт числа содержит одну или две
// десятичные цифры в так называемом двоично-десятичном коде (BCD — Binary-Coded Decimal).
// Микропроцессор хранит BCD-числа в двух форматах.

// Неупакованный формат, в этом формате каждый байт содержит одну десятичную цифру в четырех
// младших битах. Старшие четыре бита имеют нулевое значение. Это так называемая зона. Следовательно,
// диапазон представления десятичного неупакованного числа в одном байте составляет от 0 до 9.

// Упакованный формат, в этом формате каждый байт содержит две десятичные цифры.
// Десятичная цифра представляет собой двоичное значение в диапазоне от 0 до 9 размером 4 бита.
// При этом код старшей цифры числа занимает старшие 4 бита. Следовательно, диапазон
// представления десятичного упакованного числа в одном байте составляет от 00 до 99.

#include "my_types.h"
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

		template<size_type Width_in_bits>
		friend std::ostream& operator<<(std::ostream& Os, const ultralong<Width_in_bits>& number);

		template<std::size_t Width_in_bits>
		friend auto swap(ultralong<Width_in_bits>& lhs, ultralong<Width_in_bits>& rhs) noexcept;
		
		constexpr ultralong() = default;

		template<typename String_type = std::string,
			typename = std::enable_if_t<my::is_strings_v<String_type>>>
		explicit ultralong(const String_type& number_a_string) :
			ultralong(std::crbegin(number_a_string), std::crend(number_a_string)) {}

		/************************************************************************************************************************/
		template<std::size_t N>
		explicit ultralong(const char(&number_a_string)[N]) :
			ultralong(std::string{ number_a_string }) {}

		template<std::size_t N>
		explicit ultralong(const wchar_t(&number_a_string)[N]) :
			ultralong(std::wstring{ number_a_string }) {}

		template<std::size_t N>
		explicit ultralong(const char8_t(&number_a_string)[N]) :
			ultralong(std::u8string{ number_a_string }) {}

		template<std::size_t N>
		explicit ultralong(const char16_t(&number_a_string)[N]) :
			ultralong(std::u16string{ number_a_string }) {}

		template<std::size_t N>
		explicit ultralong(const char32_t(&number_a_string)[N]) :
			ultralong(std::u32string{ number_a_string }) {}

		/************************************************************************************************************************/
		template<typename Type, typename = std::enable_if_t<std::is_integral_v<Type>>>
		explicit ultralong(const std::vector<Type>& number_a_vector) :
			ultralong(std::crbegin(number_a_vector), std::crend(number_a_vector)) {}

		template<typename Type, typename = std::enable_if_t<std::is_integral_v<Type>>>
		explicit ultralong(std::initializer_list<Type> number_a_list) :
			ultralong(std::crbegin(number_a_list), std::crend(number_a_list)) {}
		
		/************************************************************************************************************************/
		template<typename Iterator>
		ultralong(Iterator first, Iterator last) : ultralong()
		{
			try {
				check(first, last);
			}
			catch (const std::exception& err) {
				throw err;
			}
			auto number = construct(first, last);
			std::swap(number_, number);
		}

		/************************************************************************************************************************/
		explicit ultralong(const ultralong& number) : ultralong()
		{
			bits::copy(number.number_, number_);
		}
		
		ultralong& operator=(const ultralong& number)
		{
			if (number_ != number.number_)
				bits::copy(number.number_, number_);
			return *this;
		}

		ultralong(ultralong&& number) noexcept
		{
			using std::swap; // Делаем возможным выроб лучшего кандидата
			swap(*this, number);
		}
		
		ultralong& operator=(ultralong&& number) noexcept
		{
			using std::swap; // Делаем возможным выроб лучшего кандидата
			swap(*this, number);
			return *this;
		}

		~ultralong() = default;

		/************************************************************************************************************************/
		template<typename String_type = std::string,
			typename = std::enable_if_t<my::is_strings_v<String_type>>>
		auto to_string() const
		{
			using namespace std;
			String_type number{};
			number.reserve(Width_in_bits / 8ull);

			to_string_impl(back_insert_iterator<decltype(number)>(number), Width_in_bits - 8ull);
			if (const auto not_zero = find_if_not(cbegin(number), cend(number),
				[](typename iterator_traits<decltype(cbegin(number))>::value_type elem) 
				{
					return elem == static_cast<decltype(elem)>('0');
				}
			); not_zero != cend(number))
			{
				number.erase(begin(number), not_zero);
				number.shrink_to_fit();
			}
			return number;
		}		

	private:
		auto swap(ultralong& number) noexcept
		{
			std::swap(number_, number.number_);
		}

		template<typename Iterator>
		auto check(Iterator first, Iterator last)
		{
			using namespace std;
			auto is_digit = [](typename iterator_traits<Iterator>::value_type elem) 
				{
					return my::is_chars_v<decltype(elem)> ? isdigit(elem) : true;
				};

			if (find_if_not(first, last, is_digit) != last)
				throw invalid_argument{ error_message("A character cannot be converted to a digit.", __LINE__) };
			
			if (Width_in_bits / 8ull < distance(first, last))
				throw out_of_range{ error_message("The type does not have the ability to accommodate the number.", __LINE__) };
		}

		template<typename Iterator>
		auto construct(Iterator first, Iterator last)
		{
			using namespace std;
			struct number_representation
			{
				void operator()(const typename iterator_traits<Iterator>::value_type& elem)
				{
					const decltype(number_) numeric{ 
						static_cast<size_type>(is_chars_v<decay_t<decltype(elem)>> ? my::to_numeric(elem) :	elem) 
					};
					const auto shift = representation.second++ * 8ull;
					representation.first ^= numeric << shift;
				}
				pair<decltype(number_), size_type> representation{}; // {{ big-endian },  {Индекс самого младшего байта }}
			};
			const auto number = for_each(first, last, number_representation());
			return number.representation.first;
		}

		template<typename String_type>
		auto to_string_impl(std::back_insert_iterator<String_type> it, int shift) const
		{
			if (shift < 0)
				return;

			constexpr decltype(number_) mask{ 0b11111111 };
			const auto numeric = bits::to_numeric(number_ >> shift & mask);
			*it = to_char<typename String_type::value_type>(numeric);

			to_string_impl(it, shift -= 8ull);
		}

	private:
		bitset number_{};
	};

	template<std::size_t N>
	auto swap(ultralong<N>& lhs, ultralong<N>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<std::size_t N>
	std::ostream& operator<<(std::ostream& os, const ultralong<N>& number) {
		using size_type = typename ultralong<N>::size_type;
		for (size_type pos{}; pos < N; ++pos)
			os << number.number_.test(N - pos - 1ull);
		return os;
	}
}

#endif // !MY_ULTRALONG_H