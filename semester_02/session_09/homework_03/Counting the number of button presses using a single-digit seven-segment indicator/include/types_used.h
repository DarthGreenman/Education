// types_used.h

#ifndef TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT
#define TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "fox.h"

#include <stdint.h>

namespace uno {
	enum class common_pin : uint8_t { anode, cathode };

	enum class single_seven_segment_indicator : uint32_t {
		a = 0x1ul << 0u, b = 0x1ul << 4u, c = 0x1ul << 8u, d = 0x1ul << 12u, e = 0x1ul << 16u,
		f = 0x1ul << 20u, g = 0x1ul << 24u, dp = 0x1ul << 28u
	};

	constexpr decltype(auto) operator | (single_seven_segment_indicator lhs, single_seven_segment_indicator rhs) {
		return static_cast<single_seven_segment_indicator>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}
	constexpr decltype(auto) operator & (single_seven_segment_indicator lhs, single_seven_segment_indicator rhs) {
		return static_cast<single_seven_segment_indicator>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}
	constexpr decltype(auto) operator ~ (single_seven_segment_indicator a) {
		return static_cast<single_seven_segment_indicator>(~static_cast<uint32_t>(a));
	}
	constexpr decltype(auto) operator |= (single_seven_segment_indicator& lhs, single_seven_segment_indicator rhs) {
		lhs = lhs | rhs; return lhs;
	}
	constexpr decltype(auto) operator &= (single_seven_segment_indicator& lhs, single_seven_segment_indicator rhs) {
		lhs = lhs & rhs; return lhs;
	}
	constexpr decltype(auto) operator << (single_seven_segment_indicator a, uint8_t shift_value) {
		return static_cast<single_seven_segment_indicator>(static_cast<uint32_t>(a) << shift_value);
	}
	constexpr decltype(auto) has(single_seven_segment_indicator value, single_seven_segment_indicator id) {
		return static_cast<uint32_t>(value & id) != 0;
	}
	static single_seven_segment_indicator& operator++(single_seven_segment_indicator& id) {
		id = static_cast<single_seven_segment_indicator>(static_cast<uint32_t>(id) << 4u);
		return id;
	}

} /// namespace uno

#endif // !TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT