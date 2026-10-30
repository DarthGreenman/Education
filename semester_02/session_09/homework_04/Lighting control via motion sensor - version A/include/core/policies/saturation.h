/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Политика насыщения (saturation).
 *
 */

#ifndef SATURATION_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define SATURATION_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <common.h>
#include <numeric_limits.h>

namespace core {
	/**
	 * @brief Выполняет преобразование значение к целевому типу с контролем диапазона.
	 */
	struct saturation_policy {
		/*
		 * Функция возвращает максимальное значение целевого типа, если значение
		 * для преобразования больше максимального значения целевого типа.
		 * Функция возвращает минимальное значение целевого типа, если значение
		 * для преобразования меньше минимального значения целевого типа, или ноль
		 * для беззнаковых типов.
		 *
		 * Даная политика исключает переполнение внутреннего представления и широко
		 * применяется в цифровой обработке сигналов, системах управления и
		 * вычислениях с фиксированной точкой.
		 *
		 * @tparam T         целевой тип
		 * @tparam W         широкий промежуточный тип
		 * @param value [in] значение для преобразования
		 * @return           ограниченное диапазоном типа T значение
		 */
		template<typename T, typename W>
		[[nodiscard]] FORCEINLINE static constexpr auto apply(W value)
			noexcept {
			constexpr auto max = (core::numeric_limits<T>::max)();
			constexpr auto min = (core::numeric_limits<T>::min)();

			return value < min ? min : (value > max ? max : static_cast<T>(value));
		}

	}; /// !saturation_policy

} /// !namespace core

#endif /// !SATURATION_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
