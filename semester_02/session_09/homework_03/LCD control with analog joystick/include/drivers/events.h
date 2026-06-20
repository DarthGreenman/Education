/// events.h

#ifndef EVENTS_H_FOR_THE_ARDUINO_UNO_PROJECT
#define EVENTS_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "utilities/common.h"
#include <stdint.h>

namespace uno {

	namespace event {
		/******************************************************************************************
		 * @brief 
		 */
		enum class button : uint8_t {
			released, pressed
		};

		/******************************************************************************************
		 * @brief
		 */
		enum class variable_resistor : uint8_t {
			change_resistance, decreased_resistance, increased_resistance
		};

		/******************************************************************************************
		 * @brief
		 */
		enum class joystick : uint8_t {
			center,
			left, /// Увеличение сигнала - уменьшение сопротивления
			right, /// Уменьшение сигнала - увеличение сопротивления
			up, /// Увеличение сигнала - уменьшение сопротивления
			down, /// Уменьшение сигнала - увеличение сопротивления
		};

		/******************************************************************************************
		 * @brief
		 */
		struct events_on_axes {
			uno::event::joystick x{ uno::event::joystick::center };
			uno::event::joystick y{ uno::event::joystick::center };
		};

		/******************************************************************************************
		 * @brief
		 */
		enum class keypad : uint8_t {
			up, down
		};

		/******************************************************************************************
		 * @brief
		 */
		template <typename E> struct type_traits {
			using type = uint8_t;
			constexpr static uint8_t size =
				utl::is_same_v<E, uno::event::button> ? 2 :
				utl::is_same_v<E, uno::event::variable_resistor> ? 3 :
				utl::is_same_v<E, uno::event::joystick> ? 5 :
				utl::is_same_v<E, uno::event::keypad> ? 2 : 0;
		};
	}
} /// !namespace uno

#endif /// !EVENTS_H_FOR_THE_ARDUINO_UNO_PROJECT

