/// signal.h

#ifndef SIGNAL_H_FOR_THE_ARDUINO_UNO_PROJECT
#define SIGNAL_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "fox.h"
#include <stdint.h>

#ifdef _ARDUINO_STUBS
#include "arduino_stubs.h"
#else
#include <Arduino.h>
#endif

namespace uno {
	namespace signal {

		/******************************************************************************************
		 * @brief  Шаблон типов сигнала
		 * @tparam L - сингал отсутствует: 0 для аналового, цифрового и ШИМ
		 * @tparam H - максимальное значение для: аналового - 1023; ШИМ - 255; цифрового - 1
		 */
		template <auto L, auto H> struct signal {
			static_assert(L <= H, "Signal: low (L) must be <= high (H)");
			using value_type =decltype(L);
			static constexpr auto low = L;
			static constexpr auto high = H;
		};
		/// Предопределённые типы сигналов для удобства использования
		using analog = signal<static_cast<uint16_t>(0), static_cast<uint16_t>(1023)>;
		using digital = signal< static_cast<uint8_t>(0), static_cast<uint8_t>(1)>;
		using pwm = signal< static_cast<uint16_t>(0), static_cast<uint16_t>(255)>;

		/// Шаблоны для определения типа сигнала компонента
		template <typename T> struct is_analog {
			constexpr static auto value = fox::is_same<typename T::signal, analog>::value;
		};
		template <typename T> struct is_digital {
			constexpr static auto value = fox::is_same<typename T::signal, digital>::value;
		};
		template <typename T> struct is_pwm {
			constexpr static auto value = fox::is_same<typename T::signal, pwm>::value;
		};

		/// Константы для удобства использования
		template <typename T> constexpr auto Is_analog = is_analog<T>::value;
		template <typename T> constexpr auto Is_digital = is_digital<T>::value;
		template <typename T> constexpr auto Is_pwm = is_pwm<T>::value;

		/******************************************************************************************
		 * @brief  Шаблон типов режимов работы канала
		 * @tparam D - константы из Arduino.h
		 */
		template <uint8_t D> struct direct {
			static_assert(D == INPUT || D == INPUT_PULLUP || D == OUTPUT, 
				"direct: D must be one of INPUT, INPUT_PULLUP, OUTPUT");
			static constexpr auto value = D;
		};

		// Предопределённые типы режимов работы канала для удобства использования
		using input = direct<INPUT>;
		using input_pullup = direct<INPUT_PULLUP>;
		using output = direct<OUTPUT>;

		// Шаблоны для определения режима работы канала компонента
		template <typename T> struct is_input {
			constexpr static auto value = fox::is_same<typename T::direct, input>::value;
		};
		template <typename T> struct is_input_pullup {
			constexpr static auto value = fox::is_same<typename T::direct, input_pullup>::value;
		};
		template <typename T> struct is_output {
			constexpr static auto value = fox::is_same<typename T::direct, output>::value;
		};

		// Константы для удобства использования
		template <typename T> constexpr auto Is_input = is_input<T>::value;
		template <typename T> constexpr auto Is_input_pullup = is_input_pullup<T>::value;
		template <typename T> constexpr auto Is_output = is_output<T>::value;

	} /// !namespace signal

} ///! namespace uno

#endif // !SIGNAL_H_FOR_THE_ARDUINO_UNO_PROJECT
