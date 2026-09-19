/// thermistor.h

#ifndef THERMISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define THERMISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT

/*		 ┌──────────────────────────┐
ADC ────────────►│ hardware_fault_detection │
				 └────────────┬─────────────┘
							  │
						 hardware_fault
							  │
							  ▼
						 thermistor
							  │
					 ADC → R → Temperature
							  │
							  ▼
						   value()
							  │
							  ▼
					  Application Logic
							  │
							  ▼
					решение по результату

*/

#include <common.h>
#include <hysteresis.h>
#include <basic_types.h>
#include <view_interfaces.h>
#include <dividers.h>
#include <electronic_component.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {
	/**********************************************************************************************
	 * @brief Класс термистора.
	 *
	 * Предназначен для работы с термисторами, подключенными к аналоговым входам Arduino Uno.
	 * Считывает аналоговое значение с указанного пина, применяет политику вычисления (например,
	 * гистерезис) и преобразует результат в температуру в градусах Цельсия.
	 * @tparam Pin Номер пина Arduino Uno, к которому подключен термистор (A0...A5).
	 * @tparam P Политика вычисления (по умолчанию - гистерезис).
	 */
	template<
		uint8_t Pin,
		typename A,
		typename P = math::hysteresis<typename analog_signal::value_type>
	>
	class thermistor :
		public electronic_component<analog_signal, input_signal, Pin> {

	public:
		using basic_component = electronic_component<analog_signal, input_signal, Pin>;
		using signal_type = typename basic_component::signal_type;
		using temperature_calculation_algorithm = A;
		using computing_policy = P;

		static_assert(
			math::is_detector_v<computing_policy, typename signal_type::value_type> ||
			math::is_transform_v<computing_policy, typename signal_type::value_type>,
			"Computing policy must have an update method returning bool or void and a value method."
			);

		constexpr thermistor() = default;
		thermistor(const thermistor&) = delete;
		thermistor(thermistor&&) = delete;
		~thermistor() = default;

		thermistor& operator = (const thermistor&) = delete;
		thermistor& operator = (thermistor&&) = delete;
		/******************************************************************************************
		 * @brief Обновляет внутреннее состояние термистора.
		 *
		 * Считывает текущее значение с аналогового пина, применяет политику вычисления и обновляет
		 * внутреннее состояние. Если политика вычисления является детектором и возвращает false,
		 * то обновление не произошло, и функция возвращает false. В противном случае функция
		 * возвращает true.
		 * @return true, если обновление прошло успешно; false, если политика вычисления отклонила
		 *         новое значение.
		 */
		[[nodiscard]] FORCEINLINE bool update() noexcept {
			const typename signal_type::value_type signal_value{ basic_component::read() };
			if constexpr(math::is_detector_v<computing_policy, typename signal_type::value_type>) {
				if(!_policy.update(signal_value))
					return false;
			} else {
				_policy.update(signal_value);
			}
			_processed_value = _policy.value();
			return true;
		}

		/******************************************************************************************
		 * @brief Возвращает текущее значение температуры в градусах Цельсия.
		 *
		 * Вычисляет температуру на основе последнего стабильного значения сигнала, используя
		 * формулу термистора. Предполагается, что термистор подключен к аналоговому входу через
		 * резистивный делитель.
		 * @return Температура в градусах Цельсия.
		 */
		template<placement S>
		[[nodiscard]] FORCEINLINE float value() const noexcept {
			const auto ratio = voltage_divider<S, 10>{}(_processed_value);
			return temperature_calculation_algorithm{}(ratio);
		}

	private:
		computing_policy _policy{}; /// Политика вычисления (размер зависит от типа, 2, 4 или 8 байт...)

		/// Последнее стабильное значение сигнала после применения политики вычисления
		/// Размер зависит от типа сигнала (2 байта для 10-битного ADC)
		typename signal_type::value_type _processed_value{};
	};

} /// !namespace uno

#endif /// !THERMISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT

