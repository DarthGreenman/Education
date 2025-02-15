// my_ultra_long.h

#ifndef MY_ULTRA_LONG_H
#define MY_ULTRA_LONG_H

#include "my_bcd.h"
#include "my_bits_ext.h"
#include "my_numeric.h"

#include <bitset>
#include <cctype>
#include <exception>
#include <iosfwd>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

namespace  numbers {
	template<std::size_t Width, typename traits = bcd<unpacked>,
		typename = std::enable_if_t<is_bcd_v<traits>>>
	class Ultra_long {
		static_assert(!(Width % traits::width_unit), "The bit field width must be a multiple of 8 or 4.");
	public:
		using bitset = std::bitset<Width>;
		using size_type = typename traits::size_type;

		template<size_type Width, typename traits> friend
		auto to_string_view(const Ultra_long<Width, traits>& Number);
		
		template<size_type Width, typename traits> friend
		std::ostream& operator<<(std::ostream& Os, const Ultra_long<Width, traits>& Number);

		constexpr Ultra_long() = default;
		explicit Ultra_long(std::string_view Number) : Ultra_long() {
			if (Width / traits::width_unit < Number.length())
				throw std::out_of_range{ "The type does not have the ability to accommodate the number." };
			try {
				construct(std::crbegin(Number), std::crend(Number));
			}
			catch (const std::exception&) { // Cимвольный литерал не определяет цифру.
				Bits_.reset();
			}
		}

		explicit Ultra_long(const Ultra_long& Number) : Ultra_long() {
			bits_ext::copy(Number.Bits_, Bits_);
		}
		
		Ultra_long& operator=(const Ultra_long& Number) {
			if (Bits_ != Number.Bits_)
				bits_ext::copy(Number.Bits_, Bits_);
			return *this;
		}

		Ultra_long(Ultra_long&& Number) noexcept : Ultra_long() {
			using std::swap; // Делаем возможным выроб лучшего кандидата.
			swap(*this, Number);
		}
		Ultra_long& operator=(Ultra_long&& Number) noexcept {
			using std::swap; // Делаем возможным выроб лучшего кандидата.
			swap(*this, Number);
			return *this;
		}

		~Ultra_long() = default;

	public:
		// Складываем числа BCD по правилам сложения двоичных чисел, и корректируем
		// результат, дополняя тетрады из которых был перенос бита в старшую тетраду
		// и тетрады которые предсталяют цифру большую 9-ти.
		// Условия корректировки сложения:
		// 1. Значение сложения в младшем полубайте больше 1001.
		// 2. Перенос единицы в младший бит старшего полубайта.
		Ultra_long& operator+=(const Ultra_long& Number) {
			if (Number.Bits_ == 0)
				return *this;
			
			using namespace bits_ext;
			decltype(Bits_) bcd{};
			for (size_type pos{}; pos < Width; pos += traits::width_unit) {
				static constexpr decltype(Bits_) mask{ 0b1111 };
				if (auto bin = add(Bits_ >> pos & mask, Number.Bits_ >> pos & mask); bin != 0) {
					if (static constexpr decltype(Bits_) ten{ 0b1001 }; bin > ten)
						bin = adj<Width, traits>(bin);
					bcd = add(bcd, bin << pos);
				}
			}
			copy(bcd, Bits_);
			return *this;
		}

		Ultra_long& operator*=(const Ultra_long& Number) {
			static_assert(is_unpacked_v<traits>, "The multiplication operation requires an unpacked type");
			return *this;
		}

		void swap(Ultra_long& Number) noexcept {
			std::swap(Bits_, Number.Bits_);
		}

	private:
		// Конструируем полубайт - конвертируем десятичную цифру в двоичную:
		// если есть остаток от деления, то устанавливаем бит в 1.
		void construct_t(int Numeric, size_type Pos) {
			if (Numeric == 0)
				return;
			static constexpr decltype(Numeric) number_base{ 2 };
			if (Numeric % number_base)
				Bits_.set(Pos);
			construct_t(Numeric /= number_base, ++Pos);
		}
		// Конструируем число из строки. Итерируем по строке с конца, проверяем является ли данный символ
		// одной из 10 десятичных цифр: 0123456789. Если да, то передаем управление конструктору полубайтов.
		using Iter = typename std::string_view::const_reverse_iterator;
		void construct(Iter First, Iter Last) {
			for (size_type pos{}; First != Last; ++First, pos += traits::width_unit) {
				if (!(static_cast<unsigned char>(std::isdigit(*First))))
					throw std::invalid_argument{ "A character cannot be converted to a digit." };
				const auto numeric = numbers::to_numeric(*First);
				construct_t(numeric, pos); // Конструируем полубайт.
			}
		}

	private:
		bitset Bits_{};
	};


	template<std::size_t Width, typename traits>
	void swap(Ultra_long<Width, traits>& Lhs, Ultra_long<Width, traits>& Rhs) {
		Lhs.swap(Rhs);
	}

	template<std::size_t Width, typename traits>
	auto operator+(const Ultra_long<Width, traits>& Lhs, const Ultra_long<Width, traits>& Rhs) {
		std::decay_t<decltype(Lhs)> tmp{ Lhs };
		tmp += Rhs;
		return tmp;
	}

	// Конвертирует Ultra_long с строку. Сдвигает старшие биты в младшие. 
	// Если биты установленные,то получаем цифру из младшего полубайта.
	template<std::size_t Width, typename traits>
	auto to_string_view(const Ultra_long<Width, traits>& Number) {
		std::string number{};
		number.reserve(Width / traits::width_unit);
		using size_type = typename traits::size_type;
		for (size_type pos{ Width / traits::width_unit - 1_z }; pos != end_reverse_v<size_type>; --pos) {
			if (const decltype(Number.Bits_) bits{ Number.Bits_ >> traits::width_unit * pos }; bits != 0) {
				constexpr decltype(Number.Bits_) mask{ 0b1111 };
				const auto numeric = bits_ext::to_numeric(bits & mask);
				number.push_back(numbers::to_char(numeric));
			}
		}
		return number;
	}

	template<std::size_t Width, typename traits>
	std::ostream& operator<<(std::ostream& Os, const Ultra_long<Width, traits>& Number) {
		Os << "BCD";
		using size_type = typename traits::size_type;
		for (size_type pos{ Width - 1_z }; pos != end_reverse_v<size_type>; --pos) {
			if (!((pos + 1_z) % traits::width_unit))
				Os << ' ';
			Os << Number.Bits_.test(pos);
		}
		return Os;
	}
}
#endif // !MY_ULTRA_LONG_H