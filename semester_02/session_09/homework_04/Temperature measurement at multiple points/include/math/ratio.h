/// ratio.h

#ifndef RATIO_H_FOR_COMPONENT_LIBRARY_PROJECT
#define RATIO_H_FOR_COMPONENT_LIBRARY_PROJECT

#include <abs.h>
#include <exp.h>
#include <view_interfaces.h>

namespace math {

	/**********************************************************************************************
	* @brief Представление математического отношения на этапе компиляции.
	*
	* Шаблон преобразует произвольный тип, описывающий числовое значение, в универсальное
	* математическое отношение:
	*         antecedent : consequent
	*
	* Использование терминов "antecedent" (предыдущий член отношения) и "consequent" (последующий
	* член отношения) выбрано сознательно. Они отражают математическое понятие отношения и не
	* ограничивают его трактовкой исключительно как дроби. Дробь является лишь одним из способов
	* интерпретации отношения.
	* Однако многие математические объекты естественным образом выражаются именно через отношение
	* величин, поэтому библиотека использует более общее математическое понятие.
	* Для типа real_number<M, E> отношение строится следующим образом:
	*         M : 10ᴱ
	* что эквивалентно значению
	*         M / 10ᴱ
	* при интерпретации отношения как дроби.
	* Для real_number<M,E>: antecedent : consequent = M : 10ᴱ.
	*
	* Благодаря этому алгоритмы библиотеки могут работать исключительно с отношениями, не завися от
	* конкретного способа представления исходных числовых данных.
	*
	* Для использования в алгоритмах тип T обязан предоставить:
	*    • static constexpr mantissa;
	*    • static constexpr exponent.
	*
	* @tparam T Тип, описывающий вещественное значение.
	*/
	template<typename T>
	struct ratio {
		static_assert(has_static_mantissa_v<T>, "T must provide static constexpr mantissa.");
		static_assert(has_static_exponent_v<T>, "T must provide static constexpr exponent.");

		/// Первый член отношения.
		static constexpr auto antecedent = T::mantissa;
		/// Второй член отношения.
		static constexpr auto consequent = exp_v<10, abs_v<T::exponent>>;
	};

} /// !namespace math

#endif /// !defined(RATIO_H_FOR_COMPONENT_LIBRARY_PROJECT)

