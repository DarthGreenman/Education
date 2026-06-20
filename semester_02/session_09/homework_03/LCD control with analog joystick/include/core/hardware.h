/// hardware.h

#ifndef HARDWARE_NEW_H_FOR_THE_ARDUINO_UNO_PROJECT
#define HARDWARE_NEW_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "defs.h"
#include "signal.h"
#include "utilities/additional.h"
#include "utilities/common.h"

#ifdef _ARDUINO_STUBS
#include "stubs.h"
#else
#include <Arduino.h>
#endif
#include <assert.h>
#include <stdint.h>

namespace uno {

	/********************************************************************************************************
	 * @brief Базовый класс электронного компонента, подключённого к Arduino Uno.
	 *
	 * Предоставляет единый интерфейс для:
	 *  - цифровых сигналов (INPUT, INPUT_PULLUP, OUTPUT)
	 *  - аналоговых сигналов (ADC)
	 *  - PWM‑выходов (analogWrite)
	 *
	 * Класс выполняет compile‑time проверку:
	 *  - корректности типа сигнала (S)
	 *  - корректности режима сигнала (M)
	 *  - корректности номеров пинов (Pins...)
	 *
	 * @tparam S    Тип сигнала (uno::signal::digital или uno::signal::analog)
	 * @tparam M    Режим сигнала (input, input_pullup, output)
	 * @tparam Pins Номера пинов, к которым подключён компонент
	 */
	template <typename S, typename M, uint8_t... Pins>
	class electronic_component {
		/****************************************************************************************************
		 * @brief Проверка корректности типа сигнала и режима.
		 *
		 * Допустимые комбинации:
		 *  - digital + input / input_pullup / output
		 *  - analog  + input / output (PWM)
		 */
		constexpr static bool is_valid_params_v =
			(uno::signal::is_analog_v<S> || uno::signal::is_digital_v<S>) &&
			(uno::signal::is_input_v<M> || uno::signal::is_input_pullup_v<M> || uno::signal::is_output_v<M>);

		/****************************************************************************************************
		 * @brief Проверить значение номера пина - является ли допустимым для данного типа и направления
		 *        сигнала.
		 * 
		 * @note Для цифровых сигналов допустимыми являются пины 0... 19 для всех режимов;
		 *       для ШИМ-сигналов - пины 3, 5, 6, 9, 10 и 11 в режиме OUTPUT;
		 *       для аналоговых сигналов - пины 14... 19 в режиме INPUT (INPUT_PULLUP).
		 * @tparam Pin       номер пина платы
		 */
		template <uint8_t Pin>
		constexpr static bool is_valid_channel_v =
			uno::signal::is_digital_v<S>
			? Pin < 20
			: uno::signal::is_analog_v<S> && uno::signal::is_output_v<M>
			? (Pin == 3 || Pin == 5 || Pin == 6 || Pin == 9 || Pin == 10 || Pin == 11)
			: uno::signal::is_analog_v<S> && !uno::signal::is_output_v<M>
			? (Pin >= 14 && Pin <= 19) : false;

		static_assert(sizeof...(Pins) > 0, "The number of pins must be greater than zero.");
		static_assert(is_valid_params_v, "Incorrect parameters signal or direction.");
		static_assert((is_valid_channel_v<Pins> && ...), "Invalid pin for this signal, direction.");

	public:
		using signal_type = S;
		using signal_mode = M;
		/****************************************************************************************************
		 * @note Число пинов, к которым подключен компонент (для компонентов с несколькими пинами, например,
		 *       семисегментного индикатора или RGB-светодиода). */
		constexpr static auto number_of_pins = sizeof...(Pins);
		using pack = utl::pin_pack<Pins...>;

		constexpr electronic_component() = default;
		electronic_component(const electronic_component&) = delete;
		electronic_component(electronic_component&&) = delete;
		~electronic_component() = default;

		electronic_component& operator=(const electronic_component&) = delete;
		electronic_component& operator=(electronic_component&&) = default;

		/****************************************************************************************************
		 * @brief Настройка пинов - интерфейс цифрового и аналогового сигналов.
		 * 
		 * @note OUTPUT: архитектура пина подключает внутренние полевые транзисторы к шинам питания (VCC или GND),
		 *       переводя пин в режим низкого сопротивления. Теперь он может выдавать 0 или 1.
		 *       INPUT_PULLUP: пин остается входом, но внутри чипа к нему подключается встроенный подтягивающий
		 *       резистор (~20–50 кОм) до линии 5V. Это защищает вход от «в воздухе» висящих наводок.
		 *       INPUT: пин остается высокоимпедансным входом.
		 */
		FORCEINLINE void begin() const {
			(pinMode(Pins, static_cast<uint8_t>(signal_mode::value)), ...);
		}

		/****************************************************************************************************
		 * @brief Записать сигнал - интерфейс аналогового и цифрового сигналов.
		 *
		 * Функция предназначенная для устройств состоящих из одного контролируемого пина.
		 * @tparam U                режим сигнала - OUTPUT
		 * @param[in] signal_value  значение в интевале: 0 ...255 или значение 0 или 1
		 */
		template <typename U = signal_mode>
		FORCEINLINE typename utl::enable_if_t<uno::signal::is_output_v<U> && sizeof... (Pins) == 1u, void>
			write(typename signal_type::value_type signal_value) const {
			constexpr auto pin_number = []() { constexpr uint8_t pins[]{ Pins... }; return pins[0]; }();
			if constexpr(uno::signal::is_analog_v<signal_type>)
				analogWrite(pin_number, signal_value);
			else
				digitalWrite(pin_number, signal_value);
		}

		/****************************************************************************************************
		 * @brief Записать сигналы - интерфейс аналогового и цифрового сигналов.
		 * 
		 * Функция предназначенная для устройств состоящих из нескольких контролируемых пинов. 
		 * @attention Алгоритм записи требует, что бы значения входного параметра (значения в массиве) были
		 *            синхронизированные. В рамках данной библиотеке, синхронизацию выпоняет класс наследник,
		 *            используя шаблонную функцию utl::... decompose(...). 
		 * @tparam U                    режим сигнала - OUTPUT
		 * @tparam N                    число элементов массива
		 * @param[in] signal_value_map  массив значений 0... 255 или 0 или 1
		 */
		template <uint8_t N, typename U = signal_mode>
		FORCEINLINE typename utl::enable_if_t<uno::signal::is_output_v<U>&& N == sizeof...(Pins), void>
			write(const typename signal_type::value_type(&signal_value_map)[N]) const {
			write_map(signal_value_map, utl::make_index_sequence<sizeof... (Pins)>{});
		}

		/****************************************************************************************************
		 * @brief Прочитать сигнал - интерфейс аналогового и цифрового сигналов. 
		 * 
		 * Функция предназначенная для устройств состоящих из одного контролируемого пина.
		 * @tparam U режим сигнала - INPUT (INPUT_PULLUP)
		 * @return   значение в интевале: 0 ...1023
		 */
		template <typename U = signal_mode>
		[[nodiscard]] FORCEINLINE typename utl::enable_if_t<!uno::signal::is_output_v<U> && sizeof... (Pins) == 1u,
			typename signal_type::value_type> read() const {
			constexpr auto pin_number = []() { constexpr uint8_t pins[]{ Pins... }; return pins[0]; }();
			if constexpr(uno::signal::is_analog_v<signal_type>)
				return analogRead(pin_number);
			else
				return digitalRead(pin_number);
		}

		/****************************************************************************************************
		 * @brief Прочитать сигнал - интерфейс аналогового и цифрового сигналов.
		 * 
		 * Функция предназначенная для устройств состоящих из нескольких контролируемых пинов. 
		 * @tparam U              режим сигнала - INPUT (INPUT_PULLUP)
		 * @param[in] pin_number  номер пина
		 * @return                значение в интевале: 0 ...1023
		 */
		template <typename U = signal_mode>
		[[nodiscard]] FORCEINLINE typename utl::enable_if_t<!uno::signal::is_output_v<U>, typename signal_type::value_type>
			read(uint8_t pin_number) const {
			if constexpr(uno::signal::is_analog_v<signal_type>)
				return analogRead(pin_number);
			else
				return digitalRead(pin_number);
		}

	private:
		/****************************************************************************************************
		 * @brief Функция для записи карты сигналов (массива значений) в несколько пинов.
		 * 
		 * Использует распаковку параметров и fold expression для итерации по массиву значений сигналов и
		 * соответствующим пинам.
		 * @attention количество элементов массива должно быть равно размерам пакетов параметров Is, Pins
		 * @tparam N                    количество элементов массива
		 * @tparam ...Is                последовательность индексов
		 * @param[in] signal_value_map  массив значений 0... 255 или 0 или 1
		 * @param index_sequence        последовательность индексов для распаковки массива и пинов
		 */
		template <uint8_t N, uint8_t... Is>
		FORCEINLINE void write_map(const typename signal_type::value_type(&signal_value_map)[N], utl::index_sequence<Is...>) const {
			auto write_to_pin = [](typename signal_type::value_type signal_value, uint8_t pin_number) {
				if constexpr(uno::signal::is_analog_v<signal_type>)
					analogWrite(pin_number, signal_value);
				else
					digitalWrite(pin_number, signal_value);
			};
			constexpr uint8_t pins[]{ Pins... };
			(write_to_pin(signal_value_map[Is], pins[Is]), ...);
		}
	};

} /// !namespace uno

#endif /// !HARDWARE_H_FOR_THE_ARDUINO_UNO_PROJECT

