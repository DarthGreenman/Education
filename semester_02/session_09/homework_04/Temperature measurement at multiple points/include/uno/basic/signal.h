/// signal.h

#ifndef SIGNAL_H_FOR_THE_ARDUINO_UNO_PROJECT
#define SIGNAL_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <type_traits.h>

#ifdef HAL
#include <stubs.h>
#else
#include <Arduino.h>
#endif

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {

	namespace helper {

		/// Константы для максимальных значений сигналов
		constexpr uint8_t digital_max_value{ 1 };
		constexpr uint16_t analog_max_value{ 1023 };

		/********************************************************************************************
		 * @brief  Шаблон типов сигнала.
		 * Сигнал может быть аналоговым или цифровым, и его диапазон значений определяется параметрами L и H.
		 * Тип сигнала определяется типом данных L и H, которые должны быть целыми и беззнаковыми.
		 * @tparam L сингал отсутствует: 0 для аналового, цифрового
		 * @tparam H максимальное значение для: аналового - 1023; цифрового - 1
		 */
		template <auto L, auto H,
			typename = core::enable_if_t<
			core::is_integral_v<decltype(L)>&& core::is_unsigned_v<decltype(L)>&&
			core::is_integral_v<decltype(H) >&& core::is_unsigned_v<decltype(H)>&&
			core::is_same_v<decltype(L), decltype(H)>
			>
		>
		struct signal {
			static_assert(L <= H, "Signal: low (L) must be <= high (H)");

			using value_type = decltype(L);
			static constexpr auto low = L;
			static constexpr auto high = H;
		};

	} /// !namespace helper

	/// Предопределённые типы сигналов
	using analog_signal = helper::signal<decltype(helper::analog_max_value){}, helper::analog_max_value > ;
	using digital_signal = helper::signal<decltype(helper::digital_max_value){}, helper::digital_max_value > ;

	/// Константы для удобства использования
	template <typename T> 
	inline constexpr auto is_analog_signal_v = core::is_same_v<T, analog_signal>;
	template <typename T> 
	inline constexpr auto is_digital_signal_v = core::is_same_v<T, digital_signal>;

	namespace helper {
		/************************************************************************************************
		 * @brief  Шаблон типов режимов работы канала.
		 * @tparam M константы из Arduino.h или stubs.h: INPUT, INPUT_PULLUP, OUTPUT
		 */
		template <auto M, typename = core::enable_if_t<core::is_integral_v<decltype(M)>>>
		struct direct_pin_mode {
			static_assert(M == INPUT || M == INPUT_PULLUP || M == OUTPUT, "mode: M must be one of INPUT,"
				" INPUT_PULLUP, OUTPUT");
			static constexpr auto value = M;
		};
	} /// !namespace helper

	/// Предопределённые типы режимов работы канала
	using input_signal = helper::direct_pin_mode<INPUT>;
	using input_pullup_signal = helper::direct_pin_mode<INPUT_PULLUP>;
	using output_signal = helper::direct_pin_mode<OUTPUT>;

	/// Константы для удобства использования
	template <typename T>
	inline constexpr auto is_input_signal_v = core::is_same_v<T, input_signal>;
	template <typename T>
	inline constexpr auto is_input_pullup_signal_v = core::is_same_v<T, input_pullup_signal>;
	template <typename T> 
	inline constexpr auto is_output_signal_v = core::is_same_v<T, output_signal>;

} /// !namespace uno

#endif /// !defined(SIGNAL_H_FOR_THE_ARDUINO_UNO_PROJECT)
