// hardware.h

#ifndef HARDWARE_H_FOR_THE_ARDUINO_UNO_PROJECT
#define HARDWARE_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "fox.h"
#include "types_used.h"

#include <Arduino.h>

#include <assert.h>
#include <stdint.h>

namespace uno
{
template <typename Ts, uno::signal_direction Sd,
	typename = fox::enable_if<fox::is_same<Ts, uno::signal_analog>::value || fox::is_same<Ts, uno::signal_digital>::value>>
class electronic_component
{
public:
	template <typename T> struct is_signal_analog
	{
		static constexpr bool value = fox::is_same<Ts, uno::signal_analog>::value;
	};
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
	electronic_component(const electronic_component&) = default;
	electronic_component(electronic_component&&) = default;
	virtual ~electronic_component() = default;

	electronic_component& operator=(const electronic_component&) = default;
	electronic_component& operator=(electronic_component&&) = default;

	/**
	 * @brief
	 *
	 * @param signal_value
	 */
	void write(uint32_t signal_value) const
	{
		is_signal_analog<Ts>::value ? analogWrite(_pin_number, signal_value) : digitalWrite(_pin_number, signal_value);
	}
	/**
	 * @brief
	 *
	 * @return uint16_t
	 */
	uint16_t read() const { return is_signal_analog<Ts>::value ? analogRead(_pin_number) : digitalRead(_pin_number); }

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
		if (is_signal_analog<Ts>::value)
			return Sd == uno::signal_direction::input
					   ? pin_number >= 14u && pin_number <= 19u
					   : pin_number == 3u || pin_number == 5u || pin_number == 6u || (pin_number >= 9u && pin_number <= 11u);
		else
			return pin_number <= 13u;
		return false;
	}
	uint8_t _pin_number{};
};
} // namespace uno

#endif // !HARDWARE_H_FOR_THE_ARDUINO_UNO_PROJECT
