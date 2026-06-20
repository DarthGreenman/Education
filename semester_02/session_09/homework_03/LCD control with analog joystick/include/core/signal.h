/// signal.h

#ifndef SIGNAL_H_FOR_THE_ARDUINO_UNO_PROJECT
#define SIGNAL_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "utilities/common.h"
#include <stdint.h>

#ifdef _ARDUINO_STUBS
#include "stubs.h"
#else
#include <Arduino.h>
#endif

namespace uno {

	namespace signal {

		namespace helper {
			/************************************************************************************************
			 * @brief  Шаблон типов сигнала
			 * @tparam L сингал отсутствует: 0 для аналового, цифрового
			 * @tparam H максимальное значение для: аналового - 1023; цифрового - 1
			 */
			template <auto L, auto H> struct signal {
				static_assert(L <= H, "Signal: low (L) must be <= high (H)");
				using value_type = decltype(L);
				constexpr static auto low = L;
				constexpr static auto high = H;
			};

		} /// !namespace helper

		/// Предопределённые типы сигналов
		using analog = uno::signal::helper::signal<static_cast<uint16_t>(0), static_cast<uint16_t>(1023)>;
		using digital = uno::signal::helper::signal<static_cast<uint8_t>(0), static_cast<uint8_t>(1)>;

		/// Константы для удобства использования
		template <typename T> constexpr auto is_analog_v = utl::is_same_v<T, uno::signal::analog>;
		template <typename T> constexpr auto is_digital_v = utl::is_same_v<T, uno::signal::digital>;

		namespace helper {
			/************************************************************************************************
			 * @brief  Шаблон типов режимов работы канала
			 * @tparam M константы из Arduino.h
			 */
			template <auto M> struct direct_pin_mode {
				static_assert(M == INPUT || M == INPUT_PULLUP || M == OUTPUT, "mode: M must be one of INPUT,"
					" INPUT_PULLUP, OUTPUT");
				constexpr static auto value = M;

			};

		} /// !namespace helper

		/// Предопределённые типы режимов работы канала
		using input = uno::signal::helper::direct_pin_mode<INPUT>;
		using input_pullup = uno::signal::helper::direct_pin_mode<INPUT_PULLUP>;
		using output = uno::signal::helper::direct_pin_mode<OUTPUT>;
		using output_pwm = uno::signal::helper::direct_pin_mode<OUTPUT>;

		/// Константы для удобства использования
		template <typename T> constexpr auto is_input_v = utl::is_same_v<T, uno::signal::input>;
		template <typename T> constexpr auto is_input_pullup_v = utl::is_same_v<T, uno::signal::input_pullup>;
		template <typename T> constexpr auto is_output_v = utl::is_same_v<T, uno::signal::output>;
		template <typename T> constexpr auto is_output_pwm_v = utl::is_same_v<T, uno::signal::output_pwm>;

	} /// !namespace signal

	namespace helper {
		/****************************************************************************************************
		* @brief Тип подключения к общему контакту
		* anode   Common Anode, для включения компонента необходимо подать низкий уровень
		*         сигнала (LOW) на его катoд
		* cathode Common Cathode, для включения сегмента необходимо подать ВЫСОКИЙ уровень
		*         сигнала (HIGH) на его анод
		*/
		enum class common_pin_value : uint8_t { anode, cathode };
		template<auto N> struct common_pin_mode { constexpr static auto value = N; };

	} /// !namespace helper

	/// Предопределённые типы режимов подключения к общему контакту
	using common_anode = uno::helper::common_pin_mode<uno::helper::common_pin_value::anode>;
	using common_cathode = uno::helper::common_pin_mode<uno::helper::common_pin_value::cathode>;

	template <typename T> constexpr auto is_common_anode_v = utl::is_same_v<T, uno::common_anode>;
	template <typename T> constexpr auto is_common_cathode_v = utl::is_same_v<T, uno::common_cathode>;

} /// !namespace uno

#endif /// !SIGNAL_H_FOR_THE_ARDUINO_UNO_PROJECT

