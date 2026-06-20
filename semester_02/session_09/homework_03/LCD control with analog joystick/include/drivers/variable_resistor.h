// variable_resistor.h

#ifndef VARIABLE_RESISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define VARIABLE_RESISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "core/hardware.h"
#include "core/types_used.h"
#include "drivers/events.h"
#include "utilities/callbacks_table.h"
#include "utilities/common.h"
#include "utilities/filters.h"
#include <stdint.h>

namespace uno {
	/********************************************************************************************************
	* @brief Класс управления переменным резистором (потенциометром).
	*
	* Предоставляет интерфейс для чтения аналогового сигнала с фильтрацией шума АЦП на основе стратегий.	
	* @tparam Pin номер аналогового пина(АЦП каналы A0 - A5, физические пины 14 - 19).
	* @tparam F   стратегия фильтрации шума. По умолчанию - фильтр гистерезиса(uno::filter::hysteresis<>).
	*/
	template<uint8_t Pin, typename F = uno::filter::hysteresis<>>
	class variable_resistor :
		public uno::electronic_component<uno::signal::analog, uno::signal::input, Pin> {

	public:
		using event = uno::event::variable_resistor;
		using pointer_to_callback_function =
			typename uno::table_of_callback_functions<event>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = uno::table_of_callback_functions<event>;

		using basic_component =
			uno::electronic_component<uno::signal::analog, uno::signal::input, Pin>;
		using signal_type = typename basic_component::signal_type;
		using filter_strategy = F;
				
		/* @note Рекомендуемый интервал опроса АЦП планировщиком(раз в 20 мс) */
		constexpr static uint8_t default_polling_time = 20;

		constexpr variable_resistor() = default;
		variable_resistor(const variable_resistor&) = delete;
		variable_resistor(variable_resistor&&) = delete;
		~variable_resistor() = default;

		variable_resistor& operator=(const variable_resistor&) = delete;
		variable_resistor& operator=(variable_resistor&&) = delete;
		/****************************************************************************************************
		 * @brief Инициализирует аналоговый вход.
		 *
		 * Делегирует настройку режима базовому классу electronic_component.
		 * Дополнительной конфигурации для АЦП не требуется.
		 */
		void begin() { basic_component::begin(); }
		/****************************************************************************************************
		 * @brief Регистрирует обработчик события изменения сопротивления.
		 *
		 * Callback вызывается только после подтверждённого изменения значения АЦП, обработанного стратегией
		 * фильтрации.
		 * @param action Указатель на функцию-обработчик.
		 */
		void on_change(const pointer_to_callback_function action) {
			_handlers.add(event::change_resistance, action);
		}

		/****************************************************************************************************
		 * @brief Чтение последнего подтвержденного стабильного значения АЦП.
		 * @return значение сигнала в диапазоне 0...1023.
		 */
		[[nodiscard]] FORCEINLINE typename signal_type::value_type get_signal() const noexcept {
			return _signal_value;
		}

		/****************************************************************************************************
		 * @brief Обновляет состояние потенциометра и генерирует событие.
		 *
		 * Алгоритм:
		 * 1. Считывает текущее значение АЦП.
		 * 2. Передаёт его в стратегию фильтрации F.
		 * 3. Если фильтр подтверждает изменение — обновляет _signal_value.
		 * 4. Генерирует событие change_resistance.
		 *
		 * @note Рекомендуемая частота вызова — каждые 20 мс.
		 */
		void update() {
			const typename signal_type::value_type curr_signal_value = basic_component::read();	
			if(_filter(curr_signal_value, _signal_value)) { /* @note Передаем сырые данные стратегии фильтрации */
				if(const auto change = _handlers.call(event::change_resistance); change) {
					change();
				}
			}
		}

	private:
		table_of_pointers_to_callback_function _handlers{}; /// Таблица колбэков (размер мин. 8 байт)
		filter_strategy _filter{}; /// Стратегия фильтрации (размер зависит от типа, 2 или 8 байт)
		typename signal_type::value_type _signal_value{};  /// Последнее стабильное значение (uint16_t, 2 байта)
	};

} /// !namespace uno

#endif /// !VARIABLE_RESISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT

