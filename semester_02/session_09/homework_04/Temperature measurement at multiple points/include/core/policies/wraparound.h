/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Политика циклического переполнения (wraparound).
 *
 */

#ifndef WRAPAROUND_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define WRAPAROUND_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#include <common.h>

namespace core {
	/**
	* @brief Выполняет преобразование значения к целевому типу без проверки диапазона.
	*/
	struct wraparound_policy {
		/**
		 * Если значение выходит за пределы диапазона представления, старшие биты
		 * отбрасываются, а результат соответствует арифметике машинного представления.
		 *
		 * Такая семантика соответствует поведению большинства процессоров для
		 * беззнаковых целочисленных типов и полезна в низкоуровневых алгоритмах,
		 * где требуется естественная арифметика представления.
		 *
		 * @tparam T         целевой тип
		 * @tparam W         широкий промежуточный тип
		 * @param value [in] значение для преобразования
		 * @return           значение, преобразованное к типу T
		 */
		template<typename T, typename W>
		[[nodiscard]] FORCEINLINE static constexpr auto apply(W value) noexcept {
			return static_cast<T>(value);
		}
	
	}; /// !struct wraparound_policy

} /// !namespace core

#endif /// !defined(WRAPAROUND_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)
