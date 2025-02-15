/* sort.h */

#ifndef SORT_H
#define SORT_H

#include <vector>
#include <concepts>


#include "utilities.h"

namespace seq
{
	/* Концепт определяет список типов, которые может принимать
	 * параметр шаблонной функции */
	template<typename T>
	concept Appropriate_type = std::same_as<T, double> || std::is_integral<T>::value &&
		!std::same_as<T, char> && !std::same_as<T, signed char> &&
		!std::same_as<T, unsigned char> && !std::same_as<T, bool>;

	struct sort
	{
		/* СОРТИРОВКА СЛИЯНИЕМ */
		template<typename T> requires Appropriate_type<T>
		static void merge(std::vector<T>& Vec);

		template<typename T> requires Appropriate_type<T>
		static void merge(T* Arr, size_t Size);

		template<typename T> requires Appropriate_type<T>
		static void merge(T* First, T* Last);

		/* БЫСТРАЯ СОРТИРОВКА */
		template<typename T> requires Appropriate_type<T>
		static void quick(std::vector<T>& Vec);

		template<typename T> requires Appropriate_type<T>
		static void quick(T* Arr, size_t Size);

		template<typename T> requires Appropriate_type<T>
		static void quick(T* First, T* Last);

		/* СОРТИРОВКА ПОДСЧЕТОМ */
		static void count(std::vector<int>& Vec);
		static void count(int* Arr, size_t Vize);
		static void count(int* First, int* Last);

	private: /* Закрытый интерфейс реализации */
		template<typename T> requires Appropriate_type<T>
		static void merge_parts(std::vector<T>& Vec,
			const std::vector<T>& First_part, const std::vector<T>& Second_part);

		template<typename T> requires Appropriate_type<T>
		static void merge_parts(T* First, T* Sep, T* Last);

		/* Быстрая сортировка по алгоритму Хоара */
		template<typename T> requires Appropriate_type<T>
		static std::pair<T*, T*> quick_split(T* First, T* Last);

		/* Рассчитывает размер массива счетчиков для интервала целых чисел,
		 * для алгоритма сортировки ПОДСЧЕТОМ */
		static size_t count_size(int Left, int Right);
	};
}

#endif /* SORT_H */