
/// led.h

#ifndef LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT
#define LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "fox.h"
#include "hardware.h"

#include <stdint.h>

namespace uno {

	/**********************************************************************************************
	 * @brief Класс для управления одним светодиодом. Этот класс наследует от базового класса
	 * electronic_component и предоставляет интерфейс для управления состоянием светодиода, включая
	 * возможность мерцать в цифровом режиме (включение-выключение) и в режиме ШИМ (регулировка яркости).
	 * Класс поддерживает оба типа сигналов - цифровой и ШИМ, что позволяет использовать его с различными
	 * типами светодиодов и конфигурациями подключения.
	 * @tparam S тип сигнала - ШИМ или цифровой
	 * @tparam Pin номер пина, к которому подключен светодиод
	 */
	template <typename S, uint8_t Pin>
	class light_emitting_diode : public uno::electronic_component<S, uno::signal::output, Pin > {
	public:
		using basic_component = uno::electronic_component<S, uno::signal::output, Pin>;
		using signal_type = typename basic_component::signal_type;

		light_emitting_diode() :
			basic_component(), _signal_value{} {}

		light_emitting_diode(const light_emitting_diode&) = delete;
		light_emitting_diode(light_emitting_diode&&) = delete;
		~light_emitting_diode() = default;

		light_emitting_diode& operator=(const light_emitting_diode&) = delete;
		light_emitting_diode& operator=(light_emitting_diode&&) = delete;

		/******************************************************************************************
		 * @brief Мигать - интерфейс сигнала ШИМ
		 * @tparam T тип сигнала - ШИМ
		 * @param signal_value значение в интевале: 0 ...255
		 */
		template <typename T = signal_type>
		typename fox::enable_if<uno::signal::Is_pwm<T>>::type blink(uint8_t signal_value) {
			_signal_value = signal_value && !_signal_value ? signal_value : signal_type::low;
			basic_component::write(_signal_value);
		}
		/******************************************************************************************
		 * @brief Мигать плавно согласовано с моментом опроса - интерфейс сигнала ШИМ
		 * @tparam T тип сигнала - ШИМ
		 * @param signal_value значение в интервале: 0 ...255
		 * @param poll_time время опроса в миллисекундах
		 * @return
		 */
		template <typename T = signal_type>
		typename fox::enable_if<uno::signal::Is_pwm<T>>::type blink(uint8_t signal_value, uint32_t poll_time) {
			const auto pause = poll_time / signal_value;
			if(_signal_value == signal_value + 1)
				/* ПРОТЕСТИРОВАТЬ НА ПРЕДМЕТ ГАРАНТИЙ ДЛЯ БЕЗЗНАКОВОГО ТИПА */
				for(--_signal_value; _signal_value != 0; --_signal_value, delay(pause))
					basic_component::write(_signal_value);
			else
				for(++_signal_value; _signal_value <= signal_value; ++_signal_value, delay(pause))
					basic_component::write(_signal_value);
		}
		/******************************************************************************************
		 * @brief Мигать - интерфейс цифрового сигнала
		 * @tparam T тип сигнала - цифровой, значение по умолчанию 0 или 1
		 */
		template <typename T = signal_type>
		typename fox::enable_if<uno::signal::Is_digital<T>>::type blink() {
			const auto signal_value = !_signal_value ? signal_type::high : signal_type::low;
			basic_component::write(_signal_value = signal_value);
		}
	private:
		typename signal_type::value_type _signal_value{};
	};
}; /// !namespace uno

#endif /// !LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT
