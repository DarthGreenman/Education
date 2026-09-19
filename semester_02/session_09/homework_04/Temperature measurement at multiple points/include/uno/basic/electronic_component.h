/// electronic_component.h

#ifndef ELECTRONIC_COMPONENT_H_FOR_THE_ARDUINO_UNO_PROJECT
#define ELECTRONIC_COMPONENT_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <basic_types.h>
#include <common.h>
#include <meta.h>

#ifdef HAL
#include <arduino_api.h>
#else
#include <Arduino.h>
#endif

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif
#include <utility.h>

namespace uno {

	/**********************************************************************************************
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
	 * @tparam S    Тип сигнала (uno::signal::digital_signal или uno::signal::analog)
	 * @tparam M    Режим сигнала (input_signal, input_pullup, output)
	 * @tparam Pins Номера пинов, к которым подключён компонент
	 */
	template <typename S, typename M, uint8_t... Pins>
	class electronic_component {
		/******************************************************************************************
		 * @brief Проверка корректности типа сигнала и режима.
		 *
		 * Допустимые комбинации:
		 *  - digital_signal + input_signal / input_pullup_signal / output_signal
		 *  - analog_signal  + input_signal / output_signal (PWM)
		 */
		static constexpr bool is_valid_params_v =
			(is_analog_signal_v<S> || is_digital_signal_v<S>) &&
			(is_input_signal_v<M> || is_input_pullup_signal_v<M> || is_output_signal_v<M>);

		/******************************************************************************************
		 * @brief Проверить значение номера пина - является ли допустимым для данного типа и
		 *        направления сигнала.
		 *
		 * Для цифровых сигналов допустимыми являются пины 0... 19 для всех режимов;
		 * для ШИМ-сигналов - пины 3, 5, 6, 9, 10 и 11 в режиме OUTPUT;
		 * для аналоговых сигналов - пины 14... 19 в режиме INPUT (INPUT_PULLUP).
		 *
		 * @tparam Pin номер пина платы
		 */
		template <uint8_t Pin>
		static constexpr bool is_valid_channel_v =
			is_digital_signal_v<S>
			? Pin < 20
			: is_analog_signal_v<S> && is_output_signal_v<M>
			? (Pin == 3 || Pin == 5 || Pin == 6 || Pin == 9 || Pin == 10 || Pin == 11)
			: is_analog_signal_v<S> && !is_output_signal_v<M>
			? (Pin >= 14 && Pin <= 19) : false;

		static_assert(sizeof...(Pins) > 0, "The number of pins must be greater than zero.");
		static_assert(is_valid_params_v, "Incorrect parameters signal or direction.");
		static_assert((is_valid_channel_v<Pins> && ...), "Invalid pin for this signal, direction.");

	public:
		using signal_type = S;
		using signal_mode = M;

		/******************************************************************************************
		 * Число пинов, к которым подключен компонент (для компонентов с несколькими пинами,
		 * например, семисегментного индикатора или RGB-светодиода).
		 */
		static constexpr auto number_of_pins = sizeof...(Pins);
		using package_pins = core::package_of_values<Pins...>;

		constexpr electronic_component() = default;
		electronic_component(const electronic_component&) = delete;
		electronic_component(electronic_component&&) = delete;
		~electronic_component() = default;

		electronic_component& operator=(const electronic_component&) = delete;
		electronic_component& operator=(electronic_component&&) = default;

		/******************************************************************************************
		 * @brief Настройка пинов - интерфейс цифрового и аналогового сигналов.
		 *
		 * OUTPUT:       архитектура пина подключает внутренние полевые транзисторы к шинам питания
		 *               (VCC или GND), переводя пин в режим низкого сопротивления. Теперь он может
		 *               выдавать 0 или 1.
		 * INPUT_PULLUP: пин остается входом, но внутри чипа к нему подключается встроенный
		 *               подтягивающий резистор (~20–50 кОм) до линии 5V. Это защищает вход от
		 *               «в воздухе» висящих наводок.
		 * INPUT:        пин остается высокоимпедансным входом.
		 */
		static void begin() { (pinMode(Pins, static_cast<uint8_t>(signal_mode::value)), ...); }

		/******************************************************************************************
		 * @brief Записать сигнал - интерфейс аналогового и цифрового сигналов.
		 *
		 * Функция предназначенная для устройств состоящих из одного контролируемого пина.
		 *
		 * @tparam U                режим сигнала - OUTPUT
		 * @param[in] signal_value  значение в интевале: 0 ...255 или значение 0 или 1
		 */
		template <typename U = signal_mode>
		FORCEINLINE static core::enable_if_t<is_output_signal_v<U> && sizeof... (Pins) == 1, void>
			write(typename signal_type::value_type signal_value) {
			constexpr auto pin_number = []() { constexpr uint8_t digital_pins[]{ Pins... }; return digital_pins[0]; }();
			if constexpr(is_analog_signal_v<signal_type>)
				analogWrite(pin_number, signal_value);
			else
				digitalWrite(pin_number, signal_value);
		}

		/*******************************************************************************************
		 * @brief Записать сигнал - интерфейс аналогового и цифрового сигналов.
		 *
		 * Функция предназначенная для одномоментной записи одинакового значения сигнала на все пины
		 * устройств состоящих из нескольких контролируемых пинов.
		 *
		 * @tparam U                режим сигнала - OUTPUT
		 * @param[in] signal_value  значение в интевале: 0 ...255 или значение 0 или 1
		 */
		template <typename U = signal_mode>
		FORCEINLINE static core::enable_if_t<is_output_signal_v<U> && sizeof... (Pins) != 1, void>
			write(typename signal_type::value_type signal_value) {
			write(prepare_map(signal_value, core::make_index_sequence<sizeof... (Pins)>{})());
		}

		/******************************************************************************************
		 * @brief Записать сигнал - интерфейс аналогового и цифрового сигналов.
		 *
		 * Функция предназначенная для устройств состоящих из нескольких контролируемых пинов и
		 * прежде всего для записи сигнала в циклах. Например: опрос клавиатуры, обновление сигнала,
		 * поступающего на разряд многоразрядного цифрового индикатора.
		 *
		 * @warning Контролировать значение пина должен вызывающий код; функция проверяет значение
		 *          параметра в runtime, и если значение пина некорректное, то функция "ничего не делает".
		 * @note    Необходимо не прерывая программу, каким-то образом известить об ошибке: вывести
		 *          сообщение на экран, начать мигать индикатором, подать звуковой сигнал; и записать
		 *          информацию об ошибке в лог.
		 *
		 * @tparam U                режим сигнала - OUTPUT
		 * @param[in] signal_value  значение в интевале: 0 ...255 или значение 0 или 1
		 * @param[in] index         индекс пина в пакете значений, значения: 0 ... (sizeof... (Pins) - 1)
		 */
		template <typename U = signal_mode>
		FORCEINLINE static core::enable_if_t<is_output_signal_v<U>, void>
			write(typename signal_type::value_type signal_value, uint8_t pin_number) {
			if(!check(pin_number)) return;
			if constexpr(is_analog_signal_v<signal_type>)
				analogWrite(pin_number, signal_value);
			else
				digitalWrite(pin_number, signal_value);
		}

		/******************************************************************************************
		 * @brief Записать сигналы - интерфейс аналогового и цифрового сигналов.
		 *
		 * Функция предназначенная для устройств состоящих из нескольких контролируемых пинов.
		 *
		 * @attention Алгоритм записи требует, что бы значения входного параметра (значения в массиве) были
		 *            синхронизированные. В рамках данной библиотеке, синхронизацию выпоняет класс наследник,
		 *            используя шаблонную функцию math::... decompose(...).
		 *
		 * @tparam U                    режим сигнала - OUTPUT
		 * @tparam B                    число элементов массива
		 * @param[in] signal_value_map  массив значений 0... 255 или 0 или 1
		 */
		template <uint8_t N, typename U = signal_mode>
		FORCEINLINE static core::enable_if_t<is_output_signal_v<U>&& N == sizeof...(Pins), void>
			write(const typename signal_type::value_type(&signal_value_map)[N]) {
			write_map(signal_value_map, core::make_index_sequence<sizeof... (Pins)>{});
		}

		/******************************************************************************************
		 * @brief Прочитать сигнал - интерфейс аналогового и цифрового сигналов.
		 *
		 * Функция предназначенная для устройств состоящих из одного контролируемого пина.
		 *
		 * @tparam U режим сигнала - INPUT (INPUT_PULLUP)
		 * @return   значение в интевале: 0 ...1023
		 */
		template <typename U = signal_mode>
		[[nodiscard]] FORCEINLINE static
			core::enable_if_t<!is_output_signal_v<U> && sizeof... (Pins) == 1u,
			typename signal_type::value_type> read() {
			constexpr auto pin_number = []() { constexpr uint8_t digital_pins[]{ Pins... }; return digital_pins[0]; }();
			if constexpr(is_analog_signal_v<signal_type>)
				return analogRead(pin_number);
			else
				return digitalRead(pin_number);
		}

		/******************************************************************************************
		 * @brief Прочитать сигнал - интерфейс аналогового и цифрового сигналов.
		 *
		 * Функция предназначенная для устройств состоящих из нескольких контролируемых пинов.
		 *
		 * @tparam U              режим сигнала - INPUT (INPUT_PULLUP)
		 * @param[in] pin_number  номер пина
		 * @return                значение в интевале: 0 ...1023
		 */
		template <typename U = signal_mode>
		[[nodiscard]] FORCEINLINE static
			core::enable_if_t<!is_output_signal_v<U> && sizeof... (Pins) != 1u,
			typename signal_type::value_type> read(uint8_t pin_number) {
			if constexpr(is_analog_signal_v<signal_type>)
				return analogRead(pin_number);
			else
				return digitalRead(pin_number);
		}

	protected:
		[[nodiscard]] FORCEINLINE static constexpr auto get_pins_wrappers() {
			return core::wrapper_for_array<uint8_t, sizeof... (Pins)>{ Pins ... };
		}

	private:
		/******************************************************************************************
		 * @brief Функция для записи карты сигналов (массива значений) в несколько пинов.
		 *
		 * Использует распаковку параметров и fold expression для итерации по массиву значений
		 * сигналов и соответствующим пинам.
		 * @attention количество элементов массива должно быть равно размерам пакетов Is, Pins
		 *
		 * @tparam B                    количество элементов массива
		 * @tparam ...Is                последовательность индексов
		 * @param[in] signal_value_map  массив значений 0... 255 или 0 или 1
		 * @param index_sequence        последовательность индексов для распаковки массива и пинов
		 */
		template <uint8_t N, auto... Is>
		FORCEINLINE static void write_map(const typename signal_type::value_type(&signal_value_map)[N],
			core::index_sequence<Is...>) {
			auto write_to_pin = [](typename signal_type::value_type signal_value, uint8_t pin_number) {
				if constexpr(is_analog_signal_v<signal_type>)
					analogWrite(pin_number, signal_value);
				else
					digitalWrite(pin_number, signal_value);
			};
			constexpr uint8_t digital_pins[]{ Pins... };
			(write_to_pin(signal_value_map[Is], digital_pins[Is]), ...);
		}

		/**********************************************************************************************
		 * @brief Формирует массив одинаковых значений сигнала.
		 *
		 * Создаёт массив фиксированного размера, каждый элемент которого содержит
		 * переданное значение сигнала.
		 *
		 * @tparam Is          Последовательность индексов, определяющая размер массива.
		 * @param signal_value Значение сигнала, которым необходимо заполнить массив.
		 * @return             Объект-обёртку, содержащий массив одинаковых значений.
		 */
		template <auto... Is>
		FORCEINLINE static auto prepare_map(typename signal_type::value_type signal_value,
			core::index_sequence<Is...>) {
			auto fill = [signal_value](uint64_t i) {return signal_value; };
			return core::wrapper_for_array<typename signal_type::value_type,
				sizeof...(Is)>{	fill(Is)... };
		}

		/**********************************************************************************************
		 * @brief Проверяет наличие вывода в списке привязанных выводов компонента.
		 *
		 * @param pin_number Номер проверяемого вывода.
		 * @retval true      Вывод принадлежит компоненту.
		 * @retval false     Вывод отсутствует среди привязанных выводов.
		 */
		static constexpr bool check(uint8_t pin_number) { return ((Pins == pin_number) || ...); }
	};

} /// !namespace uno

#endif /// !defined(ELECTRONIC_COMPONENT_H_FOR_THE_ARDUINO_UNO_PROJECT)


