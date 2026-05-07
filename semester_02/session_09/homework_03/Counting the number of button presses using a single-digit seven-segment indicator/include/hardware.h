// hardware.h
/*
 * ЦИФРОВЫЕ ПИНЫ
 * Arduino Uno имеет 14 цифровых пинов с обозначениями от 0 до 13. Эти пины можно использовать как входы или выходы,
 * чтобы контролировать подключенные устройства, например, светодиоды, кнопки, реле и т.д.
 *
 * PWM: обозначаются со знаком ~ пины 3, 5, 6, 9, 10, и 11 поддерживают ШИМ-управление (широтно-импульсную модуляцию),
 * что позволяет плавно регулировать мощность на таких устройствах, как светодиоды и сервоприводы.
 *
 * Стандартные цифровые пины 2, 4, 7, 8, 12 и 13 не поддерживают ШИМ, но могут использоваться для подачи и считывания
 * цифровых сигналов (включить/выключить). Их можно применять для различных задач: подключение реле, светодиодов или кнопок.
 *
 * RX и TX: Пины 0 (RX) и 1 (TX) используются для передачи данных через последовательный порт, что позволяет Arduino Uno
 * обмениваться данными с компьютером или другими устройствами. Эти пины работают с логическими уровнями 5 В,
 * поэтому для подключения модулей, работающих на 3.3 В, потребуется понижающий преобразователь уровня.
 *
 * АНАЛОГОВЫЕ ПИНЫ
 * На плате 6 аналоговых входов, обозначенных как A0 - A5. Эти пины позволяют измерять напряжение от аналоговых датчиков
 * (например, датчиков температуры, освещенности и т.д.) и преобразовывать его в цифровое значение для обработки программой.
 * Эти пины работают с логическими уровнями 5 В, поэтому для подключения модулей, работающих на 3.3 В, потребуется понижающий
 * преобразователь уровня.
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
	template <typename T>
	static constexpr auto Is_valid_signal = uno::signal::Is_analog<T> || uno::signal::Is_digital<T> || uno::signal::Is_pwm<T>;

	template <typename T>
	static constexpr auto Is_valid_direct = uno::signal::Is_input<T> || uno::signal::Is_input_pullup<T> || uno::signal::Is_output<T>;

	template <typename Ts, typename Ds>
	static constexpr auto Is_valid_params = Is_valid_signal<Ts> && Is_valid_direct<Ds>;

	/**
	 * @brief  Проверить соответствуют ли параметры пина параметрам соединения
	 * @tparam Ts аналоговый, ШИМ или цифровой сигнал
	 * @tparam Ds направление сигнала
	 * @param pin_number номер пина платы
	 */
	template <typename Ts, typename Ds>
	constexpr static bool is_valid_channel(uint8_t pin_number) {
		return uno::signal::Is_digital<Ts>
			? pin_number >= 0 && pin_number <= 13
			: uno::signal::Is_pwm<Ts> && uno::signal::Is_output<Ds>
			? pin_number == 3 || pin_number == 5 || pin_number == 6 || pin_number == 9 || pin_number == 10 || pin_number == 11
			: uno::signal::Is_analog<Ts> && uno::signal::Is_input<Ds>
			? pin_number >= 14 && pin_number <= 19
			: false;
	}
	/**
	 * @brief
	 * @tparam Ts
	 * @tparam Ds
	 * @tparam ...P
	 */
	template <typename Ts, typename Ds, uint8_t N>
	class electronic_component {
		static_assert(Is_valid_params<Ts, Ds>, "Incorrect parameters.");

	public:
		constexpr electronic_component() = default;
		explicit electronic_component(uint8_t pin_number) { set(pin_number); }

		electronic_component(const electronic_component&) = default;
		electronic_component(electronic_component&&) = default;
		virtual ~electronic_component() = default;

		electronic_component& operator=(const electronic_component&) = default;
		electronic_component& operator=(electronic_component&&) = default;


		void set(uint8_t pin_number, uint8_t position = 0) {
			assert(position >= 0u && position < N && "Position out of range.");
			_pins[position] = pin_number;
			pinMode(pin_number, Ds::value);
		}
		/**
		 * @brief Записать сигнала - интерфейс сигнала ШИМ
		 * @tparam T тип сигнала - ШИМ
		 * @param signal_value значение в интевале: 0 ...1023
		 */
		template <typename T = Ts>
		typename fox::enable_if<uno::signal::Is_analog<T> || uno::signal::Is_pwm<T>>::type
			write(uint16_t signal_value, uint8_t position = 0) const { analogWrite(_pins[position], signal_value); }
		/**
		 * @brief Записать сигнала - интерфейс цифрового сигнала
		 * @tparam T тип сигнала - цифровой
		 * @param signal_value значение 0 или 1
		 */
		template <typename T = Ts>
		typename fox::enable_if<!(uno::signal::Is_analog<T> || uno::signal::Is_pwm<T>)>::type
			write(uint8_t signal_value, uint8_t position = 0) const { digitalWrite(_pins[position], signal_value); }

		/**
		 * @brief Чтение сигнала - интерфейс аналогового сигнала
		 * @tparam T тип сигнала - аналоговый
		 * @return значение в интевале: 0 ...1023
		 */
		template <typename T = Ts>
		typename fox::enable_if<uno::signal::Is_analog<T> || uno::signal::Is_pwm<T>, uint16_t>::type
			read(uint8_t position = 0) const { return analogRead(_pins[position]); }
		/**
		 * @brief Чтение сигнала - интерфейс цифрового сигнала
		 * @tparam T тип сигнала - цифровой
		 * @return значение 0 или 1
		 */
		template <typename T = Ts>
		typename fox::enable_if<!(uno::signal::Is_analog<T> || uno::signal::Is_pwm<T>), uint8_t>::type
			read(uint8_t position = 0) const { return digitalRead(_pins[position]); }

	private:
		uint8_t _pins[N]{};
	};
} // namespace uno

#endif // !HARDWARE_H_FOR_THE_ARDUINO_UNO_PROJECT
