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

#include <bitset>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <algorithm>

namespace my
{
	auto error_message(const std::string& message, int line)
	{
		return std::string{ message + "\nline: " + std::to_string(line) + "\nfile: " + std::string{__FILE__} + '\n'};
	}

	template<std::size_t Width>
	class ultralong
	{
		static_assert(!(Width % bit::properties_numeric<Width>::width), "The bit field width must be a multiple of 8.");
	public:
		using size_type = std::size_t;
		using properties_numeric = bit::properties_numeric<Width>;

		// Друзья //////////////////////////////////////////////////////////////////////////////////////////////////////////
		template<std::size_t Width>
		friend auto swap(ultralong<Width>& lhs, ultralong<Width>& rhs) noexcept;

		template<typename String_type, std::size_t Width>
			requires Is_string<String_type>
		friend auto to_string(const ultralong<Width>& number);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		constexpr ultralong() = default;

		// Универсальный контруктор числа из строк /////////////////////////////////////////////////////////////////////////
		template<typename String_type = std::string,
			typename = std::enable_if_t<my::is_strings_v<String_type>>>
		explicit ultralong(const String_type& number_a_string) :
			ultralong(std::crbegin(number_a_string), std::crend(number_a_string)) {}

		// Конструкторы числа из строк /////////////////////////////////////////////////////////////////////////////////////
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

		// Конструкторы числа из элементов std::vector<Type> и std::initializer_list<Type> /////////////////////////////////
		template<typename Type, 
			typename = std::enable_if_t<std::is_integral_v<Type> && !std::is_signed_v<Type>>>
		explicit ultralong(const std::vector<Type>& number_a_vector) :
			ultralong(std::crbegin(number_a_vector), std::crend(number_a_vector)) {}

		template<typename Type, 
			typename = std::enable_if_t<std::is_integral_v<Type>>>
		explicit ultralong(std::initializer_list<Type> number_a_list) :
			ultralong(std::crbegin(number_a_list), std::crend(number_a_list)) {}
		
	private:
		// Универсальный контруктор ////////////////////////////////////////////////////////////////////////////////////////
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

	public:
		explicit ultralong(const ultralong& number) : ultralong()
		{
			bit::copy(number.number_, number_);
		}
		
		ultralong& operator=(const ultralong& number)
		{
			if (number_ != number.number_)
				bit::copy(number.number_, number_);
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

		// Складываем числа BCD по правилам сложения двоичных чисел, и корректируем
		// результат, дополняя тетрады из которых был перенос бита в старшую тетраду
		// и тетрады которые предсталяют цифру большую 9-ти.
		// Условия корректировки сложения:
		// 1. Значение сложения в младшем полубайте больше 1001.
		// 2. Перенос единицы в младший бит старшего полубайта.
		ultralong& operator+=(const ultralong& number)
		{
			if (number.number_.none())
				return *this;

			auto sum = bit::add_bcd(number_, number.number_);			
			std::swap(sum, number_);
			
			return *this;
		}

		// Используем алгоритм умножения в столбик.
		// 1. Упаковываем слагаемые столбика, полученные умножением на 1 или 0 по правилам двоичной
		//    арифметики, в std::vector<std::bitset<Width>>.
		// 2. Складываем элементы столбика по правилам сложения BCD.
		ultralong& operator*=(const ultralong& number)
		{
			using namespace std;
			vector<std::bitset<Width>> sums{};
			sums.reserve(Width);
			for (auto offset = 0ull; offset < Width; ++offset)
			{
				const auto sum = number.number_.test(offset) ? number_ & ~properties_numeric::none :
					number_ & properties_numeric::none;
				sums.push_back(sum << offset);
			}

			struct multi
			{
				auto operator()(const std::bitset<Width>& elem) { mul_= bit::add_bcd(mul_, elem); }
				std::bitset<Width> mul_{};
			};
			multi mul = for_each(cbegin(sums), cend(sums), multi());
			
			std::swap(mul.mul_, number_);
			return *this;
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
					return my::is_chars_v<decltype(elem)> ? isdigit(elem) : 
						elem < 10 ? true : false;
				};

			if (find_if_not(first, last, is_digit) != last)
				throw invalid_argument{ error_message("A character cannot be converted to a digit.", __LINE__) };
			
			if (Width / properties_numeric::width < distance(first, last))
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
					const auto offset = properties_numeric::width * representation.second++;
					representation.first ^= numeric << offset;
				}
				pair<decltype(number_), size_type> representation{}; // {{ big-endian },  {Индекс самого младшего байта }}
			};
			const auto number = for_each(first, last, number_representation{});
			return number.representation.first;
		}
		auto get() const { return number_; }

	private:
		std::bitset<Width> number_{};
	};

	template<std::size_t N>
	auto swap(ultralong<N>& lhs, ultralong<N>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<std::size_t N>
	auto operator+(const ultralong<N>& lhs, const ultralong<N>& rhs)
	{
		std::decay_t<decltype(lhs)> sum{ lhs };
		sum += rhs;
		return sum;
	}

	template<std::size_t N>
	auto operator*(const ultralong<N>& lhs, const ultralong<N>& rhs)
	{
		std::decay_t<decltype(lhs)> mul{ lhs };
		mul *= rhs;
		return mul;
	}
}
#endif // !MY_ULTRALONG_H