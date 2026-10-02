/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Более широкий тип.
 *
 */

#ifndef WIDER_TYPE_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define WIDER_TYPE_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include "meta.h"
#include <numeric_limits.h>
#include <type_traits.h>

namespace core {
	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		/*
		 * Поиск более широкого типа, чем T, выполняется последовательно по
		 * списку типов. Возвращается первый тип, для которого ширина больше 
		 * ширины типа T.
		 * Если подходящий тип отсутствует, результатом поиска является void.
		 * @tparam T  базовый тип для поиска более широкого
		 * @tparam Ts список типов, в данном случае фундаментальных
		 */
		template<typename T, typename... Ts>
		struct find_wider;

		/*
		 * Рекурсивный шаг поиска. Если Head шире T, поиск завершается и 
		 * результатом становится Head. В противном случае поиск продолжается
		 * в оставшемся списке Tail.
		 * @tparam T    базовый тип для поиска более широкого
		 * @tparam Head текущий тип списка типов
		 * @tparam Tail типы списка расположенные за текущим
		 */
		template<typename T, typename Head, typename... Tail>
		struct find_wider<T, Head, Tail...> {
			using type =
				conditional_t<(numeric_limits<Head>::width > numeric_limits<T>::width),
				Head,
				typename find_wider<T, Tail...>::type>;
		};

		/*
		 * Терминальный случай рекурсии. Пустой список означает, что все 
		 * кандидаты проверены, но более широкий тип не найден.
		 */
		template<typename T>
		struct find_wider<T> { using type = void; };

		/*
		 * Формирование более широкого целочисленного типа.
		 * Поиск выполняется среди фундаментальных беззнаковых типов. После 
		 * нахождения подходящего типа его знак восстанавливается в соответствии
		 * со знаком исходного типа T. Если более широкий фундаментальный тип 
		 * отсутствует, возникает ошибка времени компиляции.
		 */
		template<typename T>
		struct make_wider_type {
			using found_type = typename helper::find_wider<
				make_unsigned_t<T>,
				unsigned char,
				unsigned short int,
				unsigned int,
				unsigned long int,
				unsigned long long int
			>::type;
			static_assert(!is_void_v<found_type>, "For type T, the requested wider type is not "
				"provided by the standard.");

			using type = conditional_t<
				is_unsigned_v<T>,
				found_type,
				make_signed_t<found_type>>;
		};

	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Предоставляет более широкий тип, чем тип T.
	 * @tparam T базовый тип для предоставления более широкого
	 */
	template<typename T>
	using make_wider_type_t = typename helper::make_wider_type<T>::type;

} /// !namespace core

#endif /// !defined(WIDER_TYPE_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)