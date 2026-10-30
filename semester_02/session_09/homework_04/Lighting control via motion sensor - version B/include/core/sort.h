/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Сортировка диапазона последовательности элементов.
 * 
 * Функции сортирует диапазон в соответствии с заданным отношением порядка.
 * Диапазон [first, last) должен быть отсортирован таким образом,чтобы для любых
 * двух элементов a и b, где a расположен перед b, выполнялось: 
 * ```!comp(b, a)``` *
 * Предикат comp должен задавать строгое слабое упорядочивание (strict weak 
 * ordering). Компаратор определяет только порядок элементов. Он не определяет 
 * стабильность сортировки. Эквивалентными считаются элементы a и b, для 
 * которых одновременно выполняется:
 * ```!comp(a, b) && !comp(b, a)```
 */
 
#ifndef SORT_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define SORT_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <functional.h>
#include <utility.h>

namespace core {
	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename I, typename Compare = less<>>
		constexpr void unstable_sort_impl(I first, I last, Compare comp = Compare{})
			noexcept {
			for(; first != last; ++first) {
				for(auto next = first; ++next != last; ) {
					if(!comp(*first, *next)) {
						swap(*first, *next);
					}
				}
			}
		}

		template<typename I, typename Compare = less<>>
		constexpr void stable_sort_impl(I first, I last, Compare comp = Compare{})
			noexcept {
			/*@note РЕАЛИЗОВАТЬ АЛГОРИТМ СТАБИЛЬНОЙ СОРТИРОВКИ */
		}
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Тег, определяющий стабильную сортировку.
	 *
	 * Передача объекта этого типа в sort() явно запрашивает гарантию
	 * стабильности: относительный порядок эквивалентных элементов
	 * сохраняется после сортировки.
	 */
	struct stable_sort {};

	/**
	 * @brief Сортирует диапазон с сохранением относительного порядка
	 *        эквивалентных элементов.
	 *
	 * Перегрузка реализации стабильной сортировки.
	 * @tparam I          тип итератора, указывающего на элементы диапазона
	 * @tparam Compare    тип компаратора, определяющего порядок сортировки
	 * @param first       итератор, указывающий на первый элемент диапазона
	 * @param last        итератор, указывающий на элемент за последним элементом
	 *                    диапазона
	 * @param stable_sort тег, определяющий стабильную сортировку
	 * @param comp        предикат, компаратор, определяющий порядок сортировки,
	 *                    по умолчанию используется компаратор less<>, который
	 *                    сортирует элементы по возрастанию.
	 */
	template<typename I, typename Compare = less<>>
	constexpr void sort(I first, I last, stable_sort, Compare comp = Compare{})
		noexcept {
		helper::stable_sort_impl(first, last, comp);
	}

	/**
	 * @brief Сортирует диапазон в соответствии с заданным отношением порядка.
	 *
	 * Перегрузка реализацию нестабильной сортировки. Относительный порядок 
	 * эквивалентных элементов после сортировки не гарантируется.
	 * @tparam I          тип итератора, указывающего на элементы диапазона
	 * @tparam Compare    тип компаратора, определяющего порядок сортировки
	 * @param first       итератор, указывающий на первый элемент диапазона
	 * @param last        итератор, указывающий на элемент за последним элементом
	 *                    диапазона
	 * @param comp        предикат, компаратор, определяющий порядок сортировки,
	 *                    по умолчанию используется компаратор less<>, который
	 *                    сортирует элементы по возрастанию.
	 */
	template<typename I, typename Compare = less<>>
	constexpr void sort(I first, I last, Compare comp = Compare{}) noexcept {
		helper::unstable_sort_impl(first, last, comp);
	}

} /// !namespace core


#endif /// !defined(SORT_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)

