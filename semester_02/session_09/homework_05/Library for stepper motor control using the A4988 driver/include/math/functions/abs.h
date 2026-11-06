/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Вычисление модуля значения целочисленного арифметического типа
 *        во время компиляции.
 *
 */

#ifndef ABS_H_THE_COMPONENT_LIBRARY_PROJECT
#define ABS_H_THE_COMPONENT_LIBRARY_PROJECT

#include <type_traits.h>

namespace math {
	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		/*
		 * функция предназначена исключительно для целочисленных типов. 
		 * Результат всегда имеет соответствующий беззнаковый тип, что позволяет
		 * корректно представить модуль минимального отрицательного значения 
		 * (например, int32_t(-2147483648)), который не помещается в исходный
		 * знаковый тип.
		 * Для беззнаковых типов значение возвращается без изменений.
		 * Для знаковых типов вычисление производится средствами беззнаковой 
		 * арифметики:
		 *
		 *      0u - static_cast<unsigned_type>(B)
		 *
		 * Такой подход исключает переполнение знакового типа и позволяет 
		 * корректно обработать минимальное отрицательное значение без 
		 * возникновения Undefined Behavior.
		 *
		 * @tparam N Целочисленная константа времени компиляции.
		 */
		template<auto N>
		struct abs {
			using value_type = decltype(N);
			static_assert(core::is_arithmetic_integer_v<value_type>, "Only integer types are supported.");
			using unsigned_type = core::make_unsigned_t<value_type>;
			/*
			 * Для знаковых типов результат вычисляется без использования 
			 * операции унарного минуса, что исключает переполнение для 
			 * минимального отрицательного значения.
			 */
			static constexpr unsigned_type value = []() noexcept {
				if constexpr(core::is_unsigned_v<value_type>) {
					return static_cast<unsigned_type>(N);
				} else {
					return N >= 0 ? static_cast<unsigned_type>(N)
						: unsigned_type{} - static_cast<unsigned_type>(N);
				}
			}();
		};

	} /// !namespace helper
	/// @endcond

	/**
	 * @brief    Содержит модуль числа
	 * @tparam N константа времени компиляции целочисленного арифметического 
	 *           типа, модуль которого необходимо получить
	 */
	template<auto N>
	inline constexpr auto abs_v = helper::abs<N>::value;

} /// !namespace math

#endif /// !defined(ABS_H_THE_COMPONENT_LIBRARY_PROJECT)

