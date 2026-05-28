// button.h

#ifndef BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT
#define BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "callbacks_table.h"
#include "events.h"
#include "hardware.h"
#include "types_used.h"

#include <Arduino.h>
#include <stdint.h>

namespace uno
{
template <uno::signal_direction Sd = uno::signal_direction::input_pullup>
class button : public uno::electronic_component<uno::signal_digital, Sd>
{
	static_assert(Sd != uno::signal_direction::output, "Invalid signal direction parameter value.");

public:
	using pointer_to_callback_function = typename uno::table_of_callback_functions<uno::button_events>::pointer_to_callback_function;
	using table_of_pointers_to_callback_function = uno::table_of_callback_functions<uno::button_events>;
	using event = uno::button_events;
	using basic = uno::electronic_component<uno::signal_digital, Sd>;

	constexpr button() = default;
	explicit button(uint8_t pin_number) : uno::electronic_component<uno::signal_digital, Sd>(pin_number) {}
	button(const button&) = default;
	button(button&&) = default;
	~button() = default;

	button& operator=(const button&) = default;
	button& operator=(button&&) = default;

	void pressed(const pointer_to_callback_function action) { _handlers.add_handler(event::pressed, action); }
	void release(const pointer_to_callback_function action) { _handlers.add_handler(event::release, action); }
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
	void update()
	{
		const auto curr_signal_value = !basic::read() ? uno::signal_digital::low : uno::signal_digital::high;

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

		Sd == uno::signal_direction::input_pullup ? call(uno::signal_digital::low, uno::signal_digital::high)
												  : call(uno::signal_digital::high, uno::signal_digital::low);
	}

private:
	table_of_pointers_to_callback_function _handlers{};
};
} // namespace uno

#endif // !BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT