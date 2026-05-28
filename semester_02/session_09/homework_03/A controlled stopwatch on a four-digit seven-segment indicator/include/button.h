/// button.h

#ifndef BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT
#define BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "callbacks_table.h"
#include "events.h"
#include "fox.h"
#include "hardware.h"
#include "types_used.h"

namespace uno {

	/**********************************************************************************************
	 * @brief Класс кнопки, который наследует от базового класса electronic_component и предоставляет
	 * интерфейс для обработки событий нажатия и отпускания кнопки.
	 * @tparam D направление сигнала - вход или вход с подтяжкой
	 * @tparam Pin номер пина, к которому подключена кнопка
	 * @tparam Pin должен быть цифровым входом, поддерживающим pull-up или pull-down резистор, в
	 * зависимости от способа подключения кнопки к контроллеру.
	 */
	template <typename D, uint8_t Pin,
		typename = fox::enable_if_t<uno::signal::Is_input<D> || uno::signal::Is_input_pullup<D>>>
	class button :
		public uno::electronic_component<uno::signal::digital, D, Pin> {

	public:
		using event = uno::event::button;
		using pointer_to_callback_function =
			typename uno::table_of_callback_functions<event>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = uno::table_of_callback_functions<event>;


		using basic_component = uno::electronic_component<uno::signal::digital, D, Pin>;
		using signal_type = typename basic_component::signal_type;
		using signal_direction = typename basic_component::signal_direction;

		button() : basic_component(), _handlers{}, _signal_value{ basic_component::read() } {}
		button(const button&) = delete;
		button(button&&) = delete;
		~button() = default;

		button& operator=(const button&) = delete;
		button& operator=(button&&) = delete;

		void pressed(const pointer_to_callback_function action) { _handlers.add(event::pressed, action); }
		void released(const pointer_to_callback_function action) { _handlers.add(event::released, action); }

		/******************************************************************************************
		 * @brief Обновить состояние кнопки и вызвать соответствующий обработчик событий, если
		 * состояние изменилось. Состояние кнопки определяется значением сигнала, который может быть
		 * HIGH или LOW в зависимости от способа подключения кнопки к контроллеру:
		 * pull_up   - кнопка не нажата - HIGH, кнопка нажата - LOW
		 * pull_down - кнопка не нажата - LOW, кнопка нажата - HIGH
		 * Переменная _signal_value сохраняет состояние кнопки между
		 * вызовами функции update(), который зависит от выбранного способа подключения кнопки к
		 * контроллеру (pull-up или pull-down).
		 * В дальнейшем при каждом вызове функции update() будем сравнивать
		 * текущее состояние кнопки с _signal_value, чтобы определить, изменилось ли
		 * состояние кнопки (нажата или отпущена) по сравнению с предыдущим состоянием. Если состояние
		 * изменилось, то будем обрабатывать это изменение и обновлять _signal_value.
		 */
		void update() {
			const typename signal_type::value_type curr_signal_value = basic_component::read();
			auto call = [this, curr_signal_value](typename signal_type::value_type pressed,
				typename signal_type::value_type released) {
				if(curr_signal_value != _signal_value) {
					if(curr_signal_value == pressed) {
						if(const auto handler = _handlers.call(event::pressed);	handler)
							handler();
					} else if(curr_signal_value == released) {
						if(const auto handler = _handlers.call(event::released); handler)
							handler();
					}
				}
				_signal_value = curr_signal_value;
			};

			if constexpr(uno::signal::Is_input_pullup<signal_direction>)
				call(signal_type::low, signal_type::high);
			else
				call(signal_type::high, signal_type::low);
		}

	private:
		table_of_pointers_to_callback_function _handlers{};
		typename signal_type::value_type _signal_value{};
	};
} /// !namespace uno

#endif /// !BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT
