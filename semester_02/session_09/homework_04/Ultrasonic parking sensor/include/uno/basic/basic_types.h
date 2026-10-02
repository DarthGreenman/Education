/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * brief Природа и режимы работы сигналов. 
 *
 */

#ifndef BASIC_TYPES_H_FOR_THE_ARDUINO_UNO_PROJECT
#define BASIC_TYPES_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <meta.h>
#include <type_traits.h>

#ifdef HAL
#include <arduino_def.h>
#else
#include <Arduino.h>
#endif

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {
	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		/// Константы максимальных значений сигналов
		constexpr uint8_t digital_max_value{ 1 };
		constexpr uint16_t analog_max_value{ 1023 };
		/*
		 * Сигнал может быть аналоговым или цифровым, и его значения определяется
		 * параметрами Low и High.
		 * Тип сигнала определяется типом данных Low и High, которые должны быть
		 * целыми и беззнаковыми.
		 * @tparam Low  сингал отсутствует: 0 для аналового и цифрового
		 * @tparam High максимальное значение для: аналового - 1023; цифрового - 1
		 */
		template<
			auto Low,
			auto High,
			typename = core::enable_if_t<core::is_unsigned_v<decltype(Low)>&&
			core::is_unsigned_v<decltype(High)>&& core::is_same_v<decltype(Low), decltype(High)>>
			>
			struct signal {
			static_assert(Low <= High, "Signal: low (L) must be <= high (H)");

			using value_type = decltype(Low);
			static constexpr auto low = Low;
			static constexpr auto high = High;
		};
	} /// !namespace helper
	/// @endcond

	/// Предопределённые типы сигналов
	using analog_signal = helper::signal<decltype(helper::analog_max_value){}, helper::analog_max_value > ;
	using digital_signal = helper::signal<decltype(helper::digital_max_value){}, helper::digital_max_value > ;

	/// Константы для удобства использования
	template <typename T> inline constexpr auto is_analog_signal_v = core::is_same_v<T, analog_signal>;
	template <typename T> inline constexpr auto is_digital_signal_v = core::is_same_v<T, digital_signal>;


	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		/*
		 * Режимы работы канала.
		 * @tparam Mode константы из Arduino.h или arduino_def.h:
		 *              INPUT,
		 *              INPUT_PULLUP,
		 *              OUTPUT
		 */
		template<
			auto Mode,
			typename = core::enable_if_t<core::is_integral_v<decltype(Mode)>>
		>
		struct direct_pin_mode {
			static_assert(Mode == INPUT || Mode == INPUT_PULLUP || Mode == OUTPUT, 
				"Mode must be one of INPUT, INPUT_PULLUP, OUTPUT");
			static constexpr auto value = Mode;
		};
	} /// !namespace helper
	/// @endcond

	/// Предопределённые типы режимов работы канала
	using input_signal = helper::direct_pin_mode<INPUT>;
	using input_pullup_signal = helper::direct_pin_mode<INPUT_PULLUP>;
	using output_signal = helper::direct_pin_mode<OUTPUT>;

	/// Константы для удобства использования
	template <typename T> inline constexpr auto is_input_signal_v = core::is_same_v<T, input_signal>;
	template <typename T> inline constexpr auto is_input_pullup_signal_v = core::is_same_v<T, input_pullup_signal>;
	template <typename T> inline constexpr auto is_output_signal_v = core::is_same_v<T, output_signal>;

} /// !namespace uno

#endif /// !defined(BASIC_TYPES_H_FOR_THE_ARDUINO_UNO_PROJECT)
