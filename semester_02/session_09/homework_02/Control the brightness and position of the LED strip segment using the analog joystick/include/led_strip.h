// led_strip.h

#ifndef LIGHT_EMITTING_DIODE_STRIP_H
#define LIGHT_EMITTING_DIODE_STRIP_H

#include "fox.h"
#include "hardware.h"
#include "led.h"

#include <assert.h>
#include <stdint.h>

namespace wokwi
{
	template <typename Ts, uint8_t... Pn>
	class light_emitting_diode_strip
	{
	public:
		static constexpr auto number_of_diode = sizeof...(Pn);

		constexpr light_emitting_diode_strip() = default;

		template <typename T, typename = fox::enable_if<fox::is_same<T, int>::value>>
		explicit light_emitting_diode_strip(T signal_value) : _signal_params{signal_value, 0u}
		{
			fox::expand_parameter_package{(construct(Pn), 1)...};
			_signal_params.second = _range.first;
		}
		light_emitting_diode_strip(const light_emitting_diode_strip &) = delete;
		light_emitting_diode_strip(light_emitting_diode_strip &&) = delete;
		~light_emitting_diode_strip() = default;

		light_emitting_diode_strip &operator=(const light_emitting_diode_strip &) = delete;
		light_emitting_diode_strip &operator=(light_emitting_diode_strip &&) = delete;

		void shift_right()
		{
			shift(_range.second, [this]()
				  { ++_signal_params.second; });
		}
		void shift_left()
		{
			shift(_range.first, [this]()
				  { --_signal_params.second; });
		}
		void swap();
		void change_brightness(uint16_t signal_value);

	private:
		/**
		 * @brief
		 *
		 * @param pin_number
		 */
		void construct(uint8_t pin_number) { _diodes[_signal_params.second++] = wokwi::light_emitting_diode<Ts>{pin_number, 0u}; }
		/**
		 * @brief
		 *
		 * @tparam Function
		 * @param limit
		 * @param action
		 */
		template <typename Function>
		void shift(uint8_t limit, Function action);

		uint8_t get_channel_number() const { return _signal_params.second; }
		uint16_t get_signal_value() const { return _signal_params.first; }

		wokwi::light_emitting_diode<Ts> _diodes[number_of_diode]{};
		constexpr static fox::pair<uint8_t, uint8_t> _range{static_cast<uint8_t>(-1), number_of_diode};
		fox::pair<uint16_t, uint8_t> _signal_params{};
	};

	template <typename Ts, uint8_t... Pn>
	inline void light_emitting_diode_strip<Ts, Pn...>::swap()
	{
		if (get_signal_value() && get_channel_number() == 0)
			shift(_range.second, [this]()
				  { _signal_params.second += _range.second - 1u; });
		else if (get_signal_value() && get_channel_number() == _range.second - 1u)
			shift(_range.first, [this]()
				  { _signal_params.second = 0u; });
		else
			return;
	}

	template <typename Ts, uint8_t... Pn>
	inline void light_emitting_diode_strip<Ts, Pn...>::change_brightness(uint16_t signal_value)
	{
		if (get_channel_number() != _range.first && get_channel_number() != _range.second)
			_diodes[get_channel_number()].change_brightness(_signal_params.first = signal_value);
	}

	template <typename Ts, uint8_t... Pn>
	template <typename Function>
	inline void light_emitting_diode_strip<Ts, Pn...>::shift(uint8_t limit, Function action)
	{
		assert((limit == _range.first || limit == _range.second) && "Limits out of range");
		if (get_channel_number() == limit)
			return;

		auto change_brightness = [this](uint8_t position, uint16_t signal_value)
		{
			if (position != _range.first && position != _range.second)
				_diodes[position].change_brightness(signal_value);
		};

		change_brightness(get_channel_number(), 0u);
		action();
		change_brightness(get_channel_number(), get_signal_value());
	}
} // namespace wokwi

#endif // !LIGHT_EMITTING_DIODE_STRIP_H