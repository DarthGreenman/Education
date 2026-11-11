/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Подходящий тип.
 *
 */

#ifndef SUITABLE_TYPE_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define SUITABLE_TYPE_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <numeric_limits.h>

namespace core {
	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		/*
		 * Поиск типа по ширине, выполняется последовательно по списку типов.
		 * Возвращается первый тип, ширина которого равна или больше искомой.
		 * Если подходящий тип отсутствует, результатом поиска является void.
		 * @tparam W  заданная ширина
		 * @tparam Ts список типов, в данном случае фундаментальных
		 */
		template<unsigned short int W, typename... Ts>
		struct find_type;

		/*
		 * Рекурсивный шаг поиска. Если ширина Head не меньше W, поиск
		 * завершается и результатом становится Head. В противном случае поиск
		 * продолжается в оставшемся списке Tail.
		 * @tparam W    заданная ширина
		 * @tparam Head текущий тип списка типов
		 * @tparam Tail типы списка расположенные за текущим
		 */
		template<unsigned short int W, typename Head, typename... Tail>
		struct find_type<W, Head, Tail...> {
			using type =
				conditional_t<(numeric_limits<Head>::width >= W),
				Head,
				typename find_type<W, Tail...>::type>;
		};

		/*
		 * Терминальный случай рекурсии. Пустой список означает, что все
		 * кандидаты проверены, но более широкий тип не найден.
		 */
		template<unsigned short int W>
		struct find_type<W> { using type = void; };

		/*
		 * Формирование подходящего типа.
		 * Если фундаментальный тип отсутствует, возникает ошибка времени
		 * компиляции.
		 * @tparam W      заданная ширина
		 * @tparam Signed казывает, является ли число со знаком или без знака
		 */
		template<unsigned short int W, bool Signed>
		struct make_suitable_type {
		private:
			using found_type = typename helper::find_type<
				W,
				unsigned char,
				unsigned short int,
				unsigned int,
				unsigned long int,
				unsigned long long int
			>::type;
			static_assert(!is_void_v<found_type>, "For type T, the requested wider type is not "
				"provided by the standard.");

		public:
			using type = conditional_t<
				!Signed,
				found_type,
				make_signed_t<found_type>>;
		};


	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Предоставляет подходящий тип.
	 * @tparam W      заданная ширина
	 * @tparam Signed казывает, является ли число со знаком или без знака
	 */
	template<unsigned short int W, bool Signed>
	using make_suitable_type_t = typename helper::make_suitable_type<W, Signed>::type;


} /// !namespace core

#endif /// !defined(SUITABLE_TYPE_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)
