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
		enum class sign : bool { positive = false, negative = true };
	public:
		using size_type = std::size_t;
		using value_type = std::bitset<Width>;
		using properties_numeric = bit::properties_numeric<Width>;

		// Друзья //////////////////////////////////////////////////////////////////////////////////////////////////////////
		template<std::size_t Width>
		friend auto swap(ultralong<Width>& lhs, ultralong<Width>& rhs) noexcept;

		template<typename String_type, std::size_t Width>
			requires Strings<String_type>
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
			const auto sign_pos = std::find(first, last, '-');
			try {
				check(first, last, sign_pos);
			}
			catch (const std::exception& err) {
				throw err;
			}

			auto number = construct(first, sign_pos);
			if (sign_pos != last)
				number = make_signed(number, sign::negative);

			std::swap(number_, number);
		}

		explicit ultralong(const value_type& number) :
			number_{ number } {}

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

		// Операции +, *, -, / /////////////////////////////////////////////////////////////////////////////////////////////

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

			const auto lhs = is_negative(get()) ? twos_complement(get()) : get();
			const auto rhs = is_negative(number.get()) ? twos_complement(number.get()) : number.get();

			auto sum = bit::adc(lhs, rhs);
			if (is_negative(sum))
				sum = twos_complement(sum);

			sum = bit::aaa(sum);
			std::swap(number_, sum);
			
			return *this;
		}

		template<typename Type,
			typename = std::enable_if_t<std::is_integral_v<Type>>>
		ultralong& operator+=(Type number)
		{
			const ultralong rhs{ number };
			*this += rhs;
			return *this;
		}

		ultralong& operator-=(const ultralong& number)
		{
			const ultralong rhs{ reverse_sign(number.get()) };
			*this += rhs;
			return *this;
		}

		template<typename Type,
			typename = std::enable_if_t<std::is_integral_v<Type>>>
		ultralong& operator-=(Type number)
		{
			const ultralong rhs{ number };
			*this -= rhs;
			return *this;
		}

		// Используем алгоритм умножения в столбик.
		// 1. Упаковываем слагаемые столбика, полученные умножением на 1 или 0 по правилам двоичной
		//    арифметики, в std::vector<std::bitset<Width>>.
		// 2. Складываем элементы столбика по правилам сложения BCD.
		ultralong& operator*=(const ultralong& number)
		{
			using namespace std;
			vector<value_type> sums{};
			sums.reserve(Width);
			for (auto offset = 0ull; offset < Width; ++offset)
			{
				const auto sum = number.number_.test(offset) ? number_ & ~properties_numeric::none :
					number_ & properties_numeric::none;
				sums.push_back(sum << offset);
			}

			struct multi
			{
				auto operator()(const value_type& elem) { mul_= bit::add(mul_, elem); }
				value_type mul_{};
			};

			multi mul = for_each(cbegin(sums), cend(sums), multi());
			std::swap(number_, mul.mul_);
			
			return *this;
		}

		// Операции ++, -- /////////////////////////////////////////////////////////////////////////////////////////////////
		ultralong& operator++()
		{
			*this += 1ull;
			return *this;
		}

		ultralong operator++(int)
		{
			ultralong inc{ *this };
			++(*this);
			return inc;
		}

		ultralong& operator--()
		{
			*this -= 1ull;
			return *this;
		}

		ultralong operator--(int)
		{
			ultralong dec{ *this };
			--(*this);
			return dec;
		}

	private:
		auto swap(ultralong& number) noexcept
		{
			std::swap(number_, number.number_);
		}

		template<typename Iterator>
		auto check(Iterator first, Iterator last, Iterator sign_pos)
		{
			using namespace std;
			auto is_digit = [](typename iterator_traits<Iterator>::value_type elem) 
				{
					return my::is_chars_v<decltype(elem)> ? isdigit(elem) : 
						elem < 10 ? true : false;
				};

			if (sign_pos != last && distance(sign_pos, last) != 1)
				throw invalid_argument{ error_message("Incorrect format for writing a negative number.", __LINE__) };

			if (find_if_not(first, sign_pos, is_digit) != sign_pos)
				throw invalid_argument{ error_message("A character cannot be converted to a digit.", __LINE__) };

			if (Width / properties_numeric::width - 1ull < distance(first, last))
				throw out_of_range{ error_message("The type does not have the ability to accommodate the number.", __LINE__) };
		}

		template<typename Iterator>
		auto construct(Iterator first, Iterator last)
		{
			using namespace std;
			struct number_view
			{
				void operator()(const typename iterator_traits<Iterator>::value_type& elem)
				{
					const value_type numeric{ static_cast<size_type>(is_chars_v<decay_t<decltype(elem)>> ? 
						my::to_numeric(elem) : elem) };
					const auto offset = properties_numeric::width * view.second++;
					view.first ^= numeric << offset;
				}
				pair<value_type, size_type> view{}; // {{ big-endian },  {Индекс самого младшего байта }}
			};

			const auto number = for_each(first, last, number_view{});
			return number.view.first;
		}

		auto get() const
		{
			return number_;
		}
	
		static auto is_negative(const value_type& number)
		{
			return number.test(Width - properties_numeric::width);
		}
		
		static auto make_signed(const value_type& number, sign value)
		{
			value_type signed_number{ number };
			return signed_number.set(Width - properties_numeric::width, static_cast<bool>(value));
		}

		static auto reverse_sign(const value_type& number)
		{
			return is_negative(number) ? make_signed(number, sign::positive) :
				make_signed(number, sign::negative);
		}

		static auto twos_complement(const value_type& number)
		{
			const auto signed_number = make_signed(~number, sign::negative);			
			return bit::adc(signed_number, value_type{ 0b0000'0001 });
		}

	private:
		value_type number_{};
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
	auto operator-(const ultralong<N>& lhs, const ultralong<N>& rhs)
	{
		std::decay_t<decltype(lhs)> sub{ lhs };
		sub -= rhs;
		return sub;
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