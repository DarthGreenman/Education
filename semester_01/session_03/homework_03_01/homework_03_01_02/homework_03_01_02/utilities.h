/* utilities.h */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <utility>
#include <iterator>
#include <type_traits>
#include <concepts>

namespace seq
{
	/* 
	* Концепт определяет список типов, которые может принимать
	* параметр шаблонной функции.
	*/
	template<typename T>
	concept Appropriate_type = std::is_integral<T>::value &&
		!std::same_as<T, char> && !std::same_as<T, signed char> &&
		!std::same_as<T, unsigned char> && !std::same_as<T, bool>;
	
	struct search
	{
		/*
		* При совпадении искомого значения с значением элемента массива функци 
		* возвращает указатель на первый элемент и элемент следующий за последним
		* элементом последовательности одинаковых значенийЖ в противном случае - адрес
		* памяти, расположенный за последним элементом массива.
		*/
		template<typename T, size_t N>
			requires Appropriate_type<T>
		static std::pair<const T*, const T*> binary(const T(&arr)[N], T num);
		
	private: /* Закрытый интерфейс реализации */
		/*
		* Возвращает указатель на первый элемент последовательности
		* одинаковых значений.
		*/
		template<typename T, size_t N>
			requires Appropriate_type<T>
		static const T* beginning_interval(const T(&arr)[N], const T* it);
		/*
		* Возвращает указатель на элемент следующий за последним элементом
		* последовательности одинаковых значений, или адрес памяти,
		* расположенный за последним элементом массива.
		*/
		template<typename T, size_t N>
			requires Appropriate_type<T>
		static const T* ending_interval(const T(&arr)[N], const T* it);
	};
}

namespace seq
{
	template<typename T, size_t N>
		requires Appropriate_type<T>
	std::pair<const T*, const T*> search::binary(const T(&arr)[N], T num)
	{
		if (*arr == *(arr + N - 1) && num == *arr) {
			return std::make_pair(arr, arr + N);
		}

		for (auto first = arr, last = arr + N; first != last; )
		{
			auto mid = first + (last - first) / 2;
			if (num == *mid) {
				return std::make_pair(beginning_interval(arr, mid),
					ending_interval(arr, mid));
			}
			if (*arr < *(arr + N - 1)) {
				*mid < num ? first = ++mid :
					last = mid;
			}
			else {
				*mid < num ? last = mid :
					first = ++mid;
			}
		}
		return std::make_pair(arr + N, arr + N);
	}

	template<typename T, size_t N>
		requires Appropriate_type<T>
	const T* search::beginning_interval(const T(&arr)[N], const T* it)
	{
		if (!it) {
			return arr + N;
		}

		auto first = arr, last = it;
		while (first != last)
		{
			auto mid = first + (last - first) / 2;
			*it == *mid ? last = mid :
				first = ++mid;
		}
		return first;
	}

	template<typename T, size_t N>
		requires Appropriate_type<T>
	const T* search::ending_interval(const T(&arr)[N], const T* it)
	{
		if (!it) {
			return arr + N;
		}

		auto first = it, last = arr + N;
		while (first != last)
		{
			auto mid = first + (last - first) / 2;
			*it == *mid ? first = ++mid :
				last = mid;
		}
		return last;
	}
}

#endif /* UTILITIES_H */