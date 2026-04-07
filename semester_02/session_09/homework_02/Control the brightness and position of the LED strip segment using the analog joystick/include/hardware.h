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
	template <typename Signal, wokwi::signal_direction Direction,
			  typename = fox::enable_if<fox::is_same<Signal, wokwi::signal_analog>::value || fox::is_same<Signal, wokwi::signal_digital>::value>>
	class electronic_component
	{
	protected:
		constexpr electronic_component() = default;
		explicit electronic_component(uint8_t pin_number)
			: _pin_number{pin_number}
		{
			assert(is_valid(pin_number) && "Incorrect channel parameters.");
			pinMode(pin_number, static_cast<uint8_t>(Direction));
		}
		electronic_component(const electronic_component &) = default;
		electronic_component(electronic_component &&) = default;
		virtual ~electronic_component() = default;

		electronic_component &operator=(const electronic_component &) = delete;
		electronic_component &operator=(electronic_component &&) = default;

		void write(uint16_t signal_value) const
		{
			if (fox::is_same<Signal, wokwi::signal_analog>::value)
			{
				assert(signal_value >= wokwi::signal_analog::low && signal_value <= wokwi::signal_analog::high && "Incorrect analog signal value.");
				analogWrite(_pin_number, signal_value);
			}
			else
			{
				assert((signal_value == wokwi::signal_digital::low || signal_value == wokwi::signal_digital::high) && "Incorrect digital signal value.");
				digitalWrite(_pin_number, signal_value);
			}
		}

		uint16_t read() const
		{
			return fox::is_same<Signal, wokwi::signal_analog>::value	? analogRead(_pin_number)
				   : fox::is_same<Signal, wokwi::signal_digital>::value ? digitalRead(_pin_number)
																		: 0;
		}

	private:
		static bool is_valid(uint8_t pin_number)
		{
			if (fox::is_same<Signal, wokwi::signal_analog>::value)
				return Direction == wokwi::signal_direction::input
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