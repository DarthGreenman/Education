// button.h

#ifndef BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT
#define BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "callbacks_table.h"
#include "events.h"
#include "hardware.h"
#include "types_used.h"

#ifdef _ARDUINO_STUBS
#include "arduino_stubs.h"
#else
#include <Arduino.h>
#endif
#include <stdint.h>

namespace uno {
	template <typename Ds>
	class button {
		static_assert(!uno::signal::Is_output<Ds>, "Invalid signal direction parameter value.");

	public:
		using pointer_to_callback_function =
			typename uno::table_of_callback_functions<uno::button_events>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = uno::table_of_callback_functions<uno::button_events>;
		using event = uno::button_events;

		constexpr static uint8_t number_of_channels{ 1 };

		constexpr button() = default;
		explicit button(uint8_t pin_number) : _handlers{} { _component.set(pin_number); }
		button(const button&) = default;
		button(button&&) = default;
		~button() = default;

		button& operator=(const button&) = default;
		button& operator=(button&&) = default;

		void pressed(const pointer_to_callback_function action) { _handlers.add(event::pressed, action); }
		void release(const pointer_to_callback_function action) { _handlers.add(event::release, action); }
		/**
		 * @brief pull_up   - кнопка не нажата - HIGH, кнопка нажата - LOW
		 * @brief pull_down - кнопка не нажата - LOW, кнопка нажата - HIGH
		 * Переменная prev_signal_value сохраняет состояние кнопки между
		 * вызовами функции update(), который зависит от выбранного способа подключения кнопки к
		 * контроллеру (pull-up или pull-down).
		 * В дальнейшем при каждом вызове функции update() будем сравнивать
		 * текущее состояние кнопки с prev_signal_value, чтобы определить, изменилось ли
		 * состояние кнопки (нажата или отпущена) по сравнению с предыдущим состоянием. Если состояние изменилось, то
		 * будем обрабатывать это изменение и обновлять prev_signal_value.
		 */
		void update() {
			const auto curr_signal_value = !_component.read() ? uno::signal::digital::low
				: uno::signal::digital::high;

			auto call = [this, curr_signal_value](uint8_t pressed, uint8_t release) {
				static auto prev_signal_value = static_cast<uint8_t>(-1);
				if(curr_signal_value != prev_signal_value) {
					if(prev_signal_value == pressed)
						if(_handlers.call(event::pressed)) _handlers.call(event::pressed)();
					if(prev_signal_value == release)
						if(_handlers.call(event::release)) _handlers.call(event::release)();
				}
				prev_signal_value = curr_signal_value;
			};

			uno::signal::Is_input_pullup<Ds> ? call(uno::signal::digital::low, uno::signal::digital::high)
				: call(uno::signal::digital::high, uno::signal::digital::low);
		}

	private:
		uno::electronic_component<uno::signal::digital, Ds, number_of_channels> _component{};
		table_of_pointers_to_callback_function _handlers{};
	};
} // namespace uno

#endif // !BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT