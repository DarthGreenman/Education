// button.h

#ifndef BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT
#define BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "callbacks_table.h"
#include "hardware.h"
#include "events.h"
#include "types_used.h"

#include <Arduino.h>
#include <stdint.h>

namespace wokwi
{
	template <wokwi::signal_direction Direction = wokwi::signal_direction::input_pullup>
	class button : public wokwi::electronic_component<wokwi::signal_digital, Direction>
	{
		static_assert(Direction != wokwi::signal_direction::output, "Invalid signal direction parameter value." );
	public:
		using pointer_to_callback_function =
			typename wokwi::table_of_callback_functions<wokwi::button_events>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = wokwi::table_of_callback_functions<wokwi::button_events>;
		using event = wokwi::button_events;
		using basic = wokwi::electronic_component<wokwi::signal_digital, Direction>;

		constexpr button() = default;
		explicit button(uint8_t pin_number)
			: wokwi::electronic_component<wokwi::signal_digital, Direction>(pin_number)	{}
		button(const button &) = default;
		button(button &&) = default;
		~button() = default;

		button &operator=(const button &) = default;
		button &operator=(button &&) = default;

		void pressed(const pointer_to_callback_function action) { _handlers.add_handler(event::pressed, action); }
		void release(const pointer_to_callback_function action) { _handlers.add_handler(event::release, action); }

		void update();

	private:
		table_of_pointers_to_callback_function _handlers{};
	};
	/**
	 * @brief pull_up   - кнопка не нажата - HIGH, кнопка нажата - LOW
	 * @brief pull_down - кнопка не нажата - LOW, кнопка нажата - HIGH
	 * @brief Переменная prev_signal_value сохраняет состояние кнопки между вызовами функции update().
	 * @brief который зависит от выбранного способа подключения кнопки к контроллеру (pull-up или pull-down).
	 * @brief В дальнейшем при каждом вызове функции update() будем сравнивать текущее состояние
	 * @brief кнопки с prev_signal_value, чтобы определить, изменилось ли состояние кнопки (нажата или отпущена)
	 * @brief по сравнению с предыдущим состоянием. Если состояние изменилось, то будем обрабатывать
	 * @brief это изменение и обновлять prev_signal_value.
	 */
	template <wokwi::signal_direction Direction>
	void wokwi::button<Direction>::update()
	{
		const auto curr_signal_value = !basic::read() ? wokwi::signal_digital::low
													  : wokwi::signal_digital::high;

		auto call = [this, curr_signal_value](uint8_t pressed, uint8_t release)
		{
			static auto prev_signal_value = static_cast<uint8_t>(-1);
			if (curr_signal_value != prev_signal_value)
			{
				if (prev_signal_value == pressed)
					if (_handlers.handler(event::pressed))
						_handlers.handler(event::pressed)();
				if (prev_signal_value == release)
					if (_handlers.handler(event::release))
						_handlers.handler(event::release)();
			}
			prev_signal_value = curr_signal_value;
		};

		Direction == wokwi::signal_direction::input_pullup ? call(wokwi::signal_digital::low, wokwi::signal_digital::high)
														   : call(wokwi::signal_digital::high, wokwi::signal_digital::low);
	}
} // namespace wokwi

#endif // !BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT