/// button.h

#ifndef BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT
#define BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "core/hardware.h"
#include "core/types_used.h"
#include "utilities/callbacks_table.h"
#include "utilities/common.h"
#include "utilities/filters.h"
#include "events.h"

namespace uno {

	/********************************************************************************************************
	 * @brief Класс кнопки, который наследует от базового класса electronic_component и предоставляет
	 *        интерфейс для обработки событий нажатия и отпускания кнопки.
	 * @tparam M   направление сигнала - вход или вход с подтяжкой
	 * @tparam Pin должен быть цифровым входом, поддерживающим pull-up или pull-down резистор, в зависимости
	 *             от способа подключения кнопки к контроллеру
	 * @tparam F   стратегия антидребезга, по умолчанию "мягкая" с порогом фильтрации - 5.
	 */
	template <typename M, uint8_t Pin, typename F = uno::filter::integrator<5, uno::filter::reset::soft>,
		typename = utl::enable_if_t<uno::signal::is_input_v<M> || uno::signal::is_input_pullup_v<M>>>
	class button :
		public uno::electronic_component<uno::signal::digital, M, Pin> {

	public:
		using event = uno::event::button;
		using pointer_to_callback_function =
			typename uno::table_of_callback_functions<event>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = uno::table_of_callback_functions<event>;

		using basic_component = uno::electronic_component<uno::signal::digital, M, Pin>;
		using signal_type = typename basic_component::signal_type;
		using signal_mode = typename basic_component::signal_mode;
		using debounce_strategy = F;
	
		/* @note Рекомендуемое значение частоты вызова планировщика */
		constexpr static uint8_t default_polling_time = 5;

		constexpr button() = default;
		button(const button&) = delete;
		button(button&&) = delete;
		~button() = default;

		button& operator=(const button&) = delete;
		button& operator=(button&&) = delete;
		/****************************************************************************************************
		 * @brief Инициализирует пин кнопки.
		 *
		 * Делегирует настройку режима ввода базовому классу electronic_component.
		 * В режиме input_pullup активным уровнем считается LOW.
		 */
		void begin() { basic_component::begin(); }
		/****************************************************************************************************
		 * @brief Регистрирует обработчик события нажатия кнопки.
		 * @param action Callback-функция, вызываемая при нажатии.
		 */
		void on_pressed(const pointer_to_callback_function action) { _handlers.add(event::pressed, action); }
		/****************************************************************************************************
		 * @brief Регистрирует обработчик события отпускания кнопки.
		 * @param action Callback-функция, вызываемая при отпускании.
		 */
		void on_released(const pointer_to_callback_function action) { _handlers.add(event::released, action); }

		/****************************************************************************************************
		 * @brief Обновление состояния кнопки с использованием цифровой фильтрации (Debounce).
		 *
		 * Алгоритм построен по принципу реверсивного накопительного интегратора. Частота вызова метода
		 * определяется планировщиком задач (рекомендуется каждые 5 мс).
		 */
		void update() {
			/* @note Определяем, какой уровень означает "нажата" */
			constexpr auto sample_signal_value = utl::is_same_v<signal_mode, signal::input_pullup>
				? signal_type::low : signal_type::high;

			/* @note Проверяем изменение подтвержденного состояния */
			const auto signal_value = basic_component::read();
			if(_filter(signal_value, sample_signal_value, _is_pressed)) {
				if(_is_pressed) {
					if(const auto pressed = _handlers.call(event::pressed); pressed)
						pressed();
				} else {
					if(const auto released = _handlers.call(event::released); released)
						released();
				}
			}
		}

	private:
		table_of_pointers_to_callback_function _handlers{}; /// Таблица колбэков (размер мин. 8 байт)
		debounce_strategy _filter{}; /// Стратегия антидребезга (размер 1 байт)
		bool _is_pressed{}; /// размер 1 байт
	};
} /// !namespace uno

#endif /// !BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT

