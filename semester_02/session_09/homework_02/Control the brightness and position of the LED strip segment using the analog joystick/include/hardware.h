// hardware.h

#ifndef HARDWARE_H_FOR_THE_ARDUINO_UNO_PROJECT
#define HARDWARE_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "fox.h"
#include "types_used.h"

#include <Arduino.h>

#include <assert.h>
#include <stdint.h>

namespace wokwi
{
	template <typename Ts, wokwi::signal_direction Sd,
			  typename = fox::enable_if<fox::is_same<Ts, wokwi::signal_analog>::value || fox::is_same<Ts, wokwi::signal_digital>::value>>
	class electronic_component
	{
	protected:
		constexpr electronic_component() = default;
		/**
		 * @brief Construct a new electronic component object
		 *
		 * @param pin_number
		 */
		explicit electronic_component(uint8_t pin_number) : _pin_number{pin_number}
		{
			assert(is_valid(pin_number) && "Incorrect channel parameters.");
			pinMode(pin_number, static_cast<uint8_t>(Sd));
		}
		/**
		 * @brief Construct a new electronic component object
		 *
		 * @tparam T
		 * @tparam typename
		 * @tparam int>::value>
		 * @param pin_number
		 * @param signal_value
		 */
		template <typename T, typename = fox::enable_if<fox::is_same<T, int>::value>>
		explicit electronic_component(uint8_t pin_number, T signal_value) : electronic_component(pin_number) { write(signal_value); }
		electronic_component(const electronic_component &) = default;
		electronic_component(electronic_component &&) = default;
		virtual ~electronic_component() = default;

		electronic_component &operator=(const electronic_component &) = default;
		electronic_component &operator=(electronic_component &&) = default;
		/**
		 * @brief
		 *
		 * @tparam T
		 * @tparam typename
		 * @tparam int>::value>
		 * @param signal_value
		 */
		template <typename T, typename = fox::enable_if<fox::is_same<T, int>::value>>
		void write(T signal_value) const
		{
			if (fox::is_same<Ts, wokwi::signal_analog>::value && !fox::is_same<T, bool>::value)
				analogWrite(_pin_number, signal_value);
			else if (fox::is_same<Ts, wokwi::signal_digital>::value && fox::is_same<T, bool>::value)
				digitalWrite(_pin_number, signal_value);
			else
				assert(((fox::is_same<Ts, wokwi::signal_analog>::value && !fox::is_same<T, bool>::value) ||
						(fox::is_same<Ts, wokwi::signal_digital>::value && fox::is_same<T, bool>::value)) &&
					   "Incorrect parameters signal.");
		}
		/**
		 * @brief
		 *
		 * @return uint16_t
		 */
		uint16_t read() const
		{
			return fox::is_same<Ts, wokwi::signal_analog>::value	? analogRead(_pin_number)
				   : fox::is_same<Ts, wokwi::signal_digital>::value ? digitalRead(_pin_number)
																	: 0u;
		}

	private:
		/**
		 * @brief
		 *
		 * @param pin_number
		 * @return true
		 * @return false
		 */
		static bool is_valid(uint8_t pin_number)
		{
			if (fox::is_same<Ts, wokwi::signal_analog>::value)
				return Sd == wokwi::signal_direction::input
						   ? pin_number >= 14u && pin_number <= 19u
						   : pin_number == 3u || pin_number == 5u || pin_number == 6u || (pin_number >= 9u && pin_number <= 11u);
			else
				return pin_number <= 13u;
			return false;
		}
		uint8_t _pin_number{};
	};
} // namespace wokwi

#endif // !HARDWARE_H_FOR_THE_ARDUINO_UNO_PROJECT