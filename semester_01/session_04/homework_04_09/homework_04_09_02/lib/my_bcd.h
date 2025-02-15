// my_bcd.h

#ifndef MY_BCD_H
#define MY_BCD_H

// Десятичные числа — специальный вид представления числовой информации,
// в основу которого положен принцип кодирования каждой десятичной цифры числа
// группой из четырех бит. При этом каждый байт числа содержит одну или две
// десятичные цифры в так называемом двоично-десятичном коде (BCD — Binary-Coded Decimal).
// Микропроцессор хранит BCD-числа в двух форматах.

#include <cstddef>
#include <limits>
#include <type_traits>

namespace numbers {
	constexpr auto operator "" _z(unsigned long long Number) {
		return static_cast<std::size_t>(Number);
	}

	// Неупакованный формат, в этом формате каждый байт содержит одну десятичную цифру в четырех
	// младших битах. Старшие четыре бита имеют нулевое значение. Это так называемая зона. Следовательно,
	// диапазон представления десятичного неупакованного числа в одном байте составляет от 0 до 9.
	struct unpacked {};
	// Упакованный формат, в этом формате каждый байт содержит две десятичные цифры.
	// Десятичная цифра представляет собой двоичное значение в диапазоне от 0 до 9 размером 4 бита.
	// При этом код старшей цифры числа занимает старшие 4 бита. Следовательно, диапазон
	// представления десятичного упакованного числа в одном байте составляет от 00 до 99.
	struct packed {};

	template<typename T>
	constexpr auto is_valid_type_v =
		std::is_same_v<T, packed> || std::is_same_v<T, unpacked>;

	template<typename T>
	struct is_unpacked {
		static constexpr auto value = true;
	};
	template<>
	struct is_unpacked<packed> {
		static constexpr auto value = false;
	};
	template<typename T>
	constexpr auto is_unpacked_v = is_unpacked<T>::value;

	template<typename T, 
		typename = std::enable_if_t<is_valid_type_v<T>>>
	struct bcd {
		using size_type = std::size_t;
		static constexpr auto width_unit = is_unpacked_v<T> ? 8_z : 4_z;
	};

	template<typename T> struct end_reverse {
		static constexpr auto value = std::numeric_limits<T>::max();
	};

	template<typename T>
	constexpr auto end_reverse_v = end_reverse<T>::value;

	template<typename T>
	constexpr auto is_bcd_v = 
		std::is_same_v<T, bcd<packed>> || std::is_same_v<T, bcd<unpacked>>;

	using packed_t = bcd<packed>;
	using unpacked_t = bcd<unpacked>;
}

#endif // !MY_BCD_H