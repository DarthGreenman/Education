/// hardware.h
/*************************************************************************************************************************************
 * ЦИФРОВЫЕ ПИНЫ
 * Arduino Uno имеет 14 цифровых пинов с обозначениями от 0 до 13. Эти пины можно использовать как входы или выходы, чтобы контролировать 
 * подключенные устройства, например, светодиоды, кнопки, реле и т.д.
 * PWM: обозначаются со знаком ~ пины 3, 5, 6, 9, 10, и 11 поддерживают ШИМ-управление (широтно-импульсную модуляцию), что позволяет 
 * плавно регулировать мощность на таких устройствах, как светодиоды и сервоприводы.
 * Стандартные цифровые пины 2, 4, 7, 8, 12 и 13 не поддерживают ШИМ, но могут использоваться для подачи и считывания цифровых сигналов
 * (включить/выключить). Их можно применять для различных задач: подключение реле, светодиодов или кнопок.
 * RX и TX: Пины 0 (RX) и 1 (TX) используются для передачи данных через последовательный порт, что позволяет Arduino Uno обмениваться
 * данными с компьютером или другими устройствами. Эти пины работают с логическими уровнями 5 В,поэтому для подключения модулей, работающих
 * на 3.3 В, потребуется понижающий преобразователь уровня.
 * 
 * АНАЛОГОВЫЕ ПИНЫ
 * На плате 6 аналоговых входов, обозначенных как A0 - A5. Эти пины позволяют измерять напряжение от аналоговых датчиков (например, датчиков
 * температуры, освещенности и т.д.) и преобразовывать его в цифровое значение для обработки программой. Эти пины работают с логическими
 * уровнями 5 В, поэтому для подключения модулей, работающих на 3.3 В, потребуется понижающий преобразователь уровня.
 * Разрешение: Каждый аналоговый пин может считывать значения от 0 до 1023, что соответствует входному напряжению от 0 до 5 В.
 * Особенность использования: Аналоговые пины могут также работать как цифровые, если указать их в коде как входы/выходы,
 * хотя чаще всего используются именно для аналогового ввода.
 *
 *  RX ◀ 0    0	        Цифровой ввод/вывод, Serial RX
 *  TX ▶ 1    1	        Цифровой ввод/вывод, Serial TX
 *  2         2	        Цифровой ввод/вывод
 * ~3         3	        Цифровой ввод/вывод, ШИМ
 *  4         4	        Цифровой ввод/вывод
 * ~5         5	        Цифровой ввод/вывод, ШИМ
 * ~6         6	        Цифровой ввод/вывод, ШИМ
 *  7         7	        Цифровой ввод/вывод
 *  8         8	        Цифровой ввод/вывод
 * ~9         9	        Цифровой ввод/вывод, ШИМ
 * ~10       10	        Цифровой ввод/вывод, ШИМ, SPI SS
 * ~11       11	        Цифровой ввод/вывод, ШИМ, SPI MOSI
 * 12        12	        Цифровой ввод/вывод, SPI MISO
 * 13        13	        Цифровой ввод/вывод, SPI SCK
 * GND                  Земля или V-
 * AREF      		    Пин опорного напряжения
 * SDA       4 или 18   Аналоговый пин с 8-ми битным АЦП, I2C SDA
 * SCL       A5 или 19  Аналоговый пин с 8-ми битным АЦП, I2C SCL
 * 5V					5 В или V+
 * RES					Пин перезагрузки
 * 3.3V					3.3 В
 * 5V					5 В или V+
 * GND					Земля или V-
 * GND					Земля или V-
 * VIN					Пин питания соединен с + разъема питания
 * A0        A0 или 14  Аналоговый пин с 8-ми битным АЦП
 * A1        A1 или 15  Аналоговый пин с 8-ми битным АЦП
 * A2        A2 или 16  Аналоговый пин с 8-ми битным АЦП
 * A3        A3 или 17  Аналоговый пин с 8-ми битным АЦП
 * A4        A4 или 18  Аналоговый пин с 8-ми битным АЦП, I2C SDA
 * A5        A5 или 19  Аналоговый пин с 8-ми битным АЦП, I2C SCL
 */

#ifndef HARDWARE_NEW_H_FOR_THE_ARDUINO_UNO_PROJECT
#define HARDWARE_NEW_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "fox.h"
#include "signal.h"

#ifdef _ARDUINO_STUBS
#include "arduino_stubs.h"
#else
#include <Arduino.h>
#endif
#include <assert.h>
#include <stdint.h>

namespace uno {

	/*********************************************************************************************************************************
	 * @brief Шаблон класса для представления электронного компонента, который может быть подключен к плате Arduino Uno. Этот класс
	 * предоставляет интерфейс для управления сигналами, которые могут быть аналоговыми, цифровыми или ШИМ, и для указания направления
	 * сигнала (вход или выход). Класс также проверяет корректность параметров сигнала и пинов при компиляции, обеспечивая безопасность
	 * и правильность использования компонентов.
	 * @tparam S Тип сигнала (аналоговый, цифровой или ШИМ)
	 * @tparam D Направление сигнала (вход или выход)
	 * @tparam ...Pins Номера пинов, к которым подключен компонент
	 */
	template <typename S, typename D, uint8_t... Pins>
	class electronic_component {
		/// Проверить, что тип сигнала и направление сигнала являются допустимыми
		constexpr static bool Is_valid_params = (uno::signal::Is_analog<S> || uno::signal::Is_digital<S> || uno::signal::Is_pwm<S>) &&
			(uno::signal::Is_input<D> || uno::signal::Is_input_pullup<D> || uno::signal::Is_output<D>);

		/*****************************************************************************************************************************
		 * @brief Проверить, что каждый указанный пин является допустимым для данного типа сигнала и направления сигнала. Для цифровых
		 * сигналов допустимыми являются пины с 0 по 19. Для ШИМ-сигналов допустимыми являются пины 3, 5, 6, 9, 10 и 11. Для аналоговых
		 * сигналов на входе допустимыми являются пины с 14 по 19.
		 * @param pin_number номер пина платы
		 */
		template <uint8_t Pin>
		constexpr static bool Is_valid_channel = uno::signal::Is_digital<S>
			? Pin <= 19
			: uno::signal::Is_pwm<S> && uno::signal::Is_output<D>
			? Pin == 3 || Pin == 5 || Pin == 6 || Pin == 9 || Pin == 10 || Pin == 11
			: (uno::signal::Is_analog<S> && uno::signal::Is_input<D>) || (uno::signal::Is_digital<S> && uno::signal::Is_output<D>)
			? Pin >= 14 && Pin <= 19
			: false;

		static_assert(sizeof...(Pins) > 0, "The number of pins must be greater than zero.");
		static_assert(Is_valid_params, "Incorrect parameters signal or direction.");
		static_assert((Is_valid_channel<Pins> && ...), "Invalid pin for this signal, direction.");

	public:
		using signal_type = S;
		using signal_direction = D;
		/// Число пинов, к которым подключен компонент (для компонентов с несколькими пинами, например, 
		///семисегментного индикатора или RGB-светодиода).
		constexpr static auto number_of_pins = sizeof...(Pins);

		electronic_component() { (pinMode(Pins, signal_direction::value), ...); }
		electronic_component(const electronic_component&) = delete;
		electronic_component(electronic_component&&) = delete;
		virtual ~electronic_component() = default;

		electronic_component& operator=(const electronic_component&) = delete;
		electronic_component& operator=(electronic_component&&) = default;

		/*****************************************************************************************************************************
		 * @brief Записать сигнал в контакт - интерфейс сигнала ШИМ.
		 * @tparam T тип сигнала - ШИМ
		 * @param signal_value значение в интевале: 0 ...255
		 * @param position номер контакта в компоненте, по умолчанию 0
		 * @return 
		 */
		template <typename T = signal_type>
		typename fox::enable_if<(uno::signal::Is_analog<T> || uno::signal::Is_pwm<T>) && uno::signal::Is_output<signal_direction>>::type
			write(typename T::value_type signal_value, uint8_t position = 0) const {
			if(!check_position(position)) {/* Handle the error, for example: or write the error information to the log */ return; }
			analogWrite(_pins[position], signal_value);
		}
		/*****************************************************************************************************************************
		 * @brief Записать сигналы в контакты - интерфейс сигнала ШИМ.
		 * @tparam T тип сигнала - ШИМ
		 * @tparam N число элементов массива
		 * @tparam ...I номер контакта в компоненте
		 * @param signal_values массив значений в интевале: 0 ...255
		 * @param
		 * @return
		 */
		template <uint8_t N, typename T = signal_type, uint8_t... Is>
		typename fox::enable_if<(uno::signal::Is_analog<T> || uno::signal::Is_pwm<T>) && uno::signal::Is_output<signal_direction>>::type
			write(const typename T::value_type(&signal_values)[N], fox::index_sequence<Is...>) const {
			static_assert(N == sizeof...(Is), "Inconsistent parameters.");
			(write(signal_values[Is], Is), ...);
		}

		/*****************************************************************************************************************************
		 * @brief Записать сигнал в контакт - интерфейс цифрового сигнала.
		 * @tparam T тип сигнала - цифровой
		 * @param signal_value значение 0 или 1
		 */
		template <typename T = signal_type>
		typename fox::enable_if<!(uno::signal::Is_analog<T> || uno::signal::Is_pwm<T>) && uno::signal::Is_output<signal_direction>>::type
			write(typename T::value_type signal_value, uint8_t position = 0) const {
			if(!check_position(position)) {/* Handle the error, for example: or write the error information to the log */ return; }
			digitalWrite(_pins[position], signal_value);
		}
		/*****************************************************************************************************************************
		 * @brief Записать сигналы в контакты - интерфейс цифрового сигнала.
		 * @tparam T тип сигнала - цифровой
		 * @tparam N число элементов массива
		 * @tparam ...I номер контакта в компоненте
		 * @param signal_values массив значений 0 или 1
		 * @param
		 * @return
		 */
		template <uint8_t N, typename T = signal_type, uint8_t... Is>
		typename fox::enable_if<!(uno::signal::Is_analog<T> || uno::signal::Is_pwm<T>) && uno::signal::Is_output<signal_direction>>::type
			write(const typename T::value_type(&signal_values)[N], fox::index_sequence<Is...>) const {
			static_assert(N == sizeof...(Is), "Inconsistent parameters.");
			(write(signal_values[Is], Is), ...);
		}

		/*****************************************************************************************************************************
		 * @brief Прочитать сигнал - интерфейс аналогового сигнала.
		 * @tparam T тип сигнала - аналоговый
		 * @return значение в интевале: 0 ...1023
		 */
		template <typename T = signal_type>
		typename fox::enable_if<(uno::signal::Is_analog<T> || uno::signal::Is_pwm<T>) &&
			(uno::signal::Is_input<signal_direction> || uno::signal::Is_input_pullup<signal_direction>),
			typename T::value_type>::type
			read(uint8_t position = 0) const { return !check_position(position) ? typename T::value_type{} : analogRead(_pins[position]);
		}

		/*****************************************************************************************************************************
		 * @brief Прочитать сигнал - интерфейс цифрового сигнала.
		 * @tparam T тип сигнала - цифровой
		 * @return значение 0 или 1
		 */
		template <typename T = signal_type>
		typename fox::enable_if<!(uno::signal::Is_analog<T> || uno::signal::Is_pwm<T>) &&
			(uno::signal::Is_input<signal_direction> || uno::signal::Is_input_pullup<signal_direction>),
			typename T::value_type>::type
			read(uint8_t position = 0) const {return !check_position(position) ? typename T::value_type{} : digitalRead(_pins[position]);
		}

	private:
		/******************************************************************************************************************************
		 * @brief Проверить, что позиция контакта в компоненте находится в допустимом диапазоне от 0 до number_of_pins - 1. Если позиция
		 * выходит за предел, функция возвращает false и может генерировать предупреждение или ошибку, например, с помощью assert, чтобы
		 * указать на проблему во время разработки. Это помогает предотвратить попытки доступа к несуществующим контактам компонента,
		 * что может привести к непредсказуемому поведению или повреждению оборудования.
		 * @param position номер контакта в компоненте, который нужно проверить на допустимость (0 ... number_of_pins - 1)
		 * @return 
		 */
		bool check_position(uint8_t position) const {
			if(position >= number_of_pins) { assert(false && "Position out of range.");	return false; }
			return true;
		}

		constexpr static uint8_t _pins[sizeof...(Pins)]{ Pins... };
	};
} /// !namespace uno

#endif /// !HARDWARE_H_FOR_THE_ARDUINO_UNO_PROJECT
