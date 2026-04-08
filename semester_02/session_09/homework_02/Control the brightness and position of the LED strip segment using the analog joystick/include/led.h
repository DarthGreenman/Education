// led.h

#ifndef LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT
#define LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "hardware.h"
#include "fox.h"

#include <stdint.h>

namespace wokwi
{
	template <typename Ts>
	class light_emitting_diode : public wokwi::electronic_component<Ts, wokwi::signal_direction::output>
	{
	public:
		using basic = wokwi::electronic_component<Ts, wokwi::signal_direction::output>;

		constexpr light_emitting_diode() = default;
		/**
		 * @brief Construct a new light emitting diode object
		 * 
		 * @tparam T 
		 * @tparam typename 
		 * @tparam int>::value> 
		 * @param pin_number 
		 * @param signal_value 
		 */
		template <typename T, typename = fox::enable_if<fox::is_same<T, int>::value>>
		explicit light_emitting_diode(uint8_t pin_number, T signal_value) : basic(pin_number, signal_value) {}

		light_emitting_diode(const light_emitting_diode &) = default;
		light_emitting_diode(light_emitting_diode &&) = default;
		~light_emitting_diode() = default;

		light_emitting_diode &operator=(const light_emitting_diode &) = default;
		light_emitting_diode &operator=(light_emitting_diode &&) = default;

		void blink()
		{
			!basic::read() ? basic::write(static_cast<bool>(wokwi::signal_digital::high))
						   : basic::write(static_cast<bool>(wokwi::signal_digital::low));
		}
		/**
		 * @brief 
		 * 
		 * @param signal_value 
		 */
		void blink(uint16_t signal_value)
		{
			using namespace wokwi;
			constexpr auto mid = (signal_analog::high - signal_analog::low) / 2u;
			static auto value = signal_value > mid ? signal_analog::high : signal_analog::low;

			value > mid ? change_brightness(value = signal_analog::low) : change_brightness(value = signal_analog::high);
		}
		void change_brightness(uint16_t signal_value) { basic::write(signal_value); }
	};
}; // namespace wokwi

#endif // !LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT