// led.h

#ifndef LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT
#define LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "hardware.h"
#include "fox.h"

#include <stdint.h>

namespace wokwi
{
	template <typename Signal>
	class light_emitting_diode : public wokwi::electronic_component<Signal, wokwi::signal_direction::output>
	{
	public:
		using basic = wokwi::electronic_component<Signal, wokwi::signal_direction::output>;

		constexpr light_emitting_diode() = default;
		explicit light_emitting_diode(uint8_t pin_number) : wokwi::electronic_component<Signal, wokwi::signal_direction::output>(pin_number) {}
		light_emitting_diode(const light_emitting_diode &) = default;
		light_emitting_diode(light_emitting_diode &&) = default;
		~light_emitting_diode() = default;

		light_emitting_diode &operator=(const light_emitting_diode &) = default;
		light_emitting_diode &operator=(light_emitting_diode &&) = default;

		void blink() { !basic::read() ? increase_brightness() : decrease_brightness(); }
		void blink(uint16_t signal_value);

		void increase_brightness() { basic::write(wokwi::signal_digital::high); }
		void decrease_brightness() { basic::write(wokwi::signal_digital::low); }
		void change_brightness(uint16_t signal_value) { basic::write(signal_value); }
	};
	template <typename Signal>
	inline void light_emitting_diode<Signal>::blink(uint16_t signal_value)
	{
		using namespace wokwi;
		constexpr auto mid = (signal_analog::high - signal_analog::low) / 2u;
		static auto value = signal_value > mid ? signal_analog::high : signal_analog::low;

		value > mid ? change_brightness(value = signal_analog::low) : change_brightness(value = signal_analog::high);
	}
} // namespace wokwi

#endif // !LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT