/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Экспоненциальный фильтр скользящего среднего 
 *        (Exponential Moving Average, EMA).
 *
 */

#ifndef EMA_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define EMA_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <abs.h>
#include <common.h>
#include <exp.h>
#include <ratio.h>
#include <type_traits.h>
#include <view_interfaces.h>

namespace math {

	/*
	 * Структура реализует цифровой БИХ-фильтр (IIR, Infinite Impulse Response), 
	 * в котором выходное значение вычисляется на основе текущей выборки и 
	 * предыдущего значения фильтра. Благодаря наличию обратной связи влияние 
	 * каждой выборки сохраняется, и экспоненциально уменьшается с течением времени.
	 *
	 * В отличие от фильтра SMA, EMA не требует хранения окна последних выборок,
	 * поскольку для вычисления нового значения используется только предыдущий 
	 * результат фильтрации.
	 *
	 * Основные свойства:
	 * - бесконечная импульсная характеристика (IIR);
	 * - наличие обратной связи;
	 * - постоянное потребление памяти;
	 * - низкая вычислительная сложность;
	 * - быстро реагирует на изменение входного сигнала;
	 * - обеспечивает меньшее запаздывание по сравнению с SMA при сопоставимом
	 *   уровне сглаживания;
	 * - не обладает линейной фазовой характеристикой.
	 *
	 * Степень сглаживания определяется коэффициентом α (Alpha). Чем меньше 
	 * значение α, тем сильнее подавляется шум и тем медленнее фильтр реагирует 
	 * на изменение входного сигнала. Чем больше α, тем быстрее фильтр следует 
	 * за входным сигналом, уменьшая степень сглаживания.
	 *
	 * @tparam T тип значений входных выборок
	 * @tparam C тип, который может предоставить значение коэффициента Alpha
	 */
	template<typename T, typename C>
	struct ema {
		using value_type = T;
		using coeff_type = C;
		static_assert(core::is_arithmetic_integer_v<value_type>, "Only integer types are supported.");
		static_assert(has_static_mantissa_v<coeff_type>, "C must have a static member mantissa.");
		static_assert(has_static_exponent_v<coeff_type>, "C must have a static member exponent.");
		static constexpr auto base_of_the_number_system = 10u;
		static_assert(coeff_type::mantissa > 0, "Alpha must be positive.");
		static_assert(coeff_type::mantissa < exp_v<base_of_the_number_system, abs_v<coeff_type::exponent>>,
			"Alpha must be less than 1.");
		/**
		 * @brief Метод обновляет результат фильтрации на основе новой выборки.
		 *
		 * Передача первой выборки инициализирует фильтр, устанавливая его 
		 * значение равным этой выборке.
		 * @param sample [in] значение входной выборки
		 */
		FORCEINLINE void update(value_type sample) noexcept {
			if(_first_sample) {
				_first_sample = false;
				_smoothed_value = sample;
				return;
			}
			/// Вычисление коэффициента сглаживания α.
			constexpr auto alpha = static_cast<float>(ratio<coeff_type>::antecedent) / ratio<coeff_type>::consequent;
			/// Вычисление ошибки между текущей выборкой и предыдущим сглаженным значением.
			const auto error = static_cast<float>(sample) - _smoothed_value;
			/// Вычисление корректировки на основе ошибки и коэффициента α.
			const auto correction = alpha * error;
			/// Обновление сглаженного значения с учётом корректировки.
			_smoothed_value += static_cast<value_type>(correction);
		}

		[[nodiscard]] FORCEINLINE value_type value() const noexcept { return _smoothed_value; }

	private:
		value_type _smoothed_value{}; /**< Сглаженное значение фильтра. */
		bool _first_sample{ true };   /**< Флаг, указывающий на то, что фильтр ещё не получил первую выборку. */
	};

} /// !namespace math

#endif /// !defined(EMA_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)

