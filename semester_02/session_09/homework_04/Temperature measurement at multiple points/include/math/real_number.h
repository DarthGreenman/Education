/// real_number.h

#ifndef REAL_NUMBER_H_FOR_COMPONENT_LIBRARY_PROJECT
#define REAL_NUMBER_H_FOR_COMPONENT_LIBRARY_PROJECT

#include <type_traits.h>

namespace math {

	/**********************************************************************************************
   * brief Представление вещественного числа на этапе компиляции.
   *
   * Шаблон описывает математическое вещественное значение в нормализованной десятичной форме:
   *         M × 10⁻ᴱ, где:
   *                   M — мантисса;
   *                   E — десятичный показатель степени.
   * Данный тип не хранит состояние во время выполнения программы и не выполняет никаких вычислений.
   * Его единственная задача — предоставить математическое описание константы на этапе компиляции.
   * В отличие от fixed_point, данный тип ничего не говорит о способе хранения числа в памяти. Он
   * описывает исключительно само значение, которое в дальнейшем может быть преобразовано в любое
   * необходимое представление.
   *
   * Основные области применения:
   *    • описание физических констант;
   *    • задание коэффициентов алгоритмов;
   *    • описание допустимой погрешности вычислений;
   *    • выбор параметров fixed_point;
   *    • использование в других compile-time вычислениях.
   *
   * @tparam M Мантисса числа.
   * @tparam E Десятичный показатель степени.
   * */
	template<auto M, auto E>
	struct real_number {
		static_assert(core::is_arithmetic_integer_v<decltype(M)>, "Mantissa M must be an integer type.");
		static_assert(core::is_arithmetic_integer_v<decltype(E)>, "Exponent M must be an integer type.");

		/// Мантисса десятичного представления числа.
		static constexpr auto mantissa = M;
		/// Десятичный показатель степени, значение интерпретируется как: mantissa × 10⁻ᵉˣᵖᵒⁿᵉⁿᵗ
		static constexpr auto exponent = E;
	};

} /// !namespace core

#endif /// !defined(REAL_NUMBER_H_FOR_COMPONENT_LIBRARY_PROJECT)
