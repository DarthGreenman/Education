/// events.h

#ifndef EVENTS_H_FOR_THE_ARDUINO_UNO_PROJECT
#define EVENTS_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "fox.h"
#include <stdint.h>

namespace uno {
	namespace event {
		enum class button : uint8_t {
			released, pressed
		};
		enum class variable_resistor : uint8_t {
			decreased_resistance, increased_resistance
		};
		enum class joystick : uint8_t {
			left, /// Увеличение сигнала - уменьшение сопротивления
			right, /// Уменьшение сигнала - увеличение сопротивления
			up, /// Увеличение сигнала - уменьшение сопротивления
			down, /// Уменьшение сигнала - увеличение сопротивления
			selection /// ...
		};
		enum class keypad : uint8_t {
			up, down
		};

		template <typename E> struct type_traits {
			using type = uint8_t;
			constexpr static auto size =
				fox::is_same<E, uno::event::button>::value ? 2u :
				fox::is_same<E, uno::event::variable_resistor>::value ? 2u :
				fox::is_same<E, uno::event::joystick>::value ? 5u :
				fox::is_same<E, uno::event::keypad>::value ? 2u : 0u;
		};
	}
} /// !namespace uno

#endif /// !EVENTS_H_FOR_THE_ARDUINO_UNO_PROJECT
