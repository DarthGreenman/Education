/// additional.h

#ifndef ADDITIONAL_H__FOR_THE_ARDUINO_UNO_PROJECT
#define ADDITIONAL_H__FOR_THE_ARDUINO_UNO_PROJECT

#include "core/types_used.h"
#include "common.h"
#include <stdint.h>

namespace utl {

	template <typename T, uint8_t... N>
	[[nodiscard]] constexpr inline decltype(auto) decompose(uint32_t value, uint8_t mask) noexcept {
		// static_assert(((!(N % 4) || !(N % 1)) && ...), "All bit offsets N must be multiples of 1 or 4.");
		static_assert(((N < sizeof(uint32_t) * 8) && ...), "Each N must be < number of bits in T.");

		auto decomposing = [&](uint32_t n) { return static_cast<T>((value >> n) & mask); };
		return utl::wrapper_for_array<T, sizeof...(N)>{ decomposing(N)... };
	}

	template <auto Pin, typename... Bs>
	struct find_pin;

	template <auto Pin, typename Head, typename... Tail> struct find_pin<Pin, Head, Tail...> {
		constexpr static uint8_t value = (uno::get_bind<1, Head>::value == Pin)
			? uno::get_bind<0, Head>::value : find_pin<Pin, Tail...>::value;
	};

	template <auto Pin>
	struct find_pin<Pin> { constexpr static uint8_t value = 0xff; };

	template <auto Pin, typename... Bs>
	constexpr auto find_pin_v = utl::find_pin<Pin, Bs...>::value;

	 //template <auto... Args>
	 //struct is_unique : std::true_type {};

	 //template <auto Head, auto... Tail>
	 //struct is_unique<Head, Tail...> {
	 //	constexpr static bool value = ((Head != Tail) && ...) && is_unique<Tail...>::value;
	 //};

	 //template <auto... Args>
	 //constexpr bool is_unique_v = utl::is_unique<Args...>::value;

	template <auto... Pins>
	struct pin_pack {};

	[[nodiscard]] float map(uint16_t x, uint16_t in_min, uint16_t in_max, float out_min, float out_max) {
		return static_cast<float>(x - in_min) * (out_max - out_min)
			/ static_cast<float>(in_max - in_min) + out_min;
	}

} /// !namespace utl

#endif /// !ADDITIONAL_H__FOR_THE_ARDUINO_UNO_PROJECT

