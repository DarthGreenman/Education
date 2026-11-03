/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Фильтр скользящего среднего (Simple Moving Average, SMA).
 *
 */

#ifndef SMA_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define SMA_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <common.h>
#include <exp.h>
#include <log.h>
#include <numeric_limits.h>
#include <type_traits.h>
#include <wider_type.h>

namespace math {
	/*
	 * Структура реализует цифровой КИХ-фильтр (FIR, Finite Impulse Response),
	 * вычисляющий среднее арифметическое последних B выборок входного сигнала.
	 *
	 * Основные свойства:
	 * - линейная фазовая характеристика;
	 * - конечная импульсная характеристика (FIR);
	 * - отсутствие обратной связи и гарантированная устойчивость;
	 * - задержка фазового отклика для симметричного окна составляет (N - 1) / 2 выборки;
	 * - чем больше размер окна B, тем выше степень сглаживания и тем больше
	 *   задержка реакции на изменение входного сигнала.
	 *
	 * Для повышения производительности при полном заполнении окна деление
	 * заменяется побитовым сдвигом, если размер окна N является степенью двойки.
	 * Фильтр не требует хранения всей истории сигнала — сохраняются только
	 * последние N выборок.
	 *
	 * @tparam T тип значений входных выборок
	 * @tparam N размер окна.
	 */
	template <typename T, auto N>
	struct sma {
		using value_type = T;
		using pos_type = decltype(N);
		static_assert(core::is_arithmetic_integer_v<value_type>&& core::numeric_limits<value_type>::width <= 32,
			" Only integer types with a width of no more than 32 bits are supported.");
		static_assert(core::is_arithmetic_integer_v<pos_type>, "Only integer types are supported.");
		static_assert(N > 1, "N must be greater than one");

		/**
		 * @brief Метод обновляет результат фильтрации на основе новой выборки.
		 *
		 * Первые N вызовов update(...) выполняют заполнение окна усреднения.
		 * Далее, каждая новая выборка включается в окно усреднения, а самая
		 * старая автоматически исключается из него. Благодаря этому фильтр
		 * эффективно подавляет случайный шум и высокочастотные помехи, сохраняя
		 * при этом постоянную составляющую сигнала.
		 *
		 * @param sample [in] значение входной выборки
		 */
		FORCEINLINE void update(value_type sample) noexcept {
			/// Локальная лямбда-функция для добавления новой выборки в буфер 
			/// и обновления суммы.
			auto fill_in = [this, sample]() {
				_samples[_head] = sample; /**< Сохраняем новую выборку в буфер. */
				_acc += sample;           /**< Добавляем новую выборку в сумму. */
				++_head;                  /**< Обновляем индекс следующей позиции для записи. */
			};
			/// Первоначальное заполнение окна усреднения.
			if(!_full) {
				fill_in();
				if(_head == N) {
					_full = true;
					_head = 0;
				}
				return;
			}
			_acc -= _samples[_head]; /**< Удаляем из суммы значение, покидающее окно. */
			fill_in();
			if(_head == N)
				_head = 0;
		}
		
		[[nodiscard]] FORCEINLINE value_type value() const noexcept {
			if(!_full)
				return static_cast<value_type>(_acc / _head);
			/// Если значение ширины окна число, является степенью двойки, то оптимизируем
			/// целочисленное деление битовым сдвигом.
			if constexpr(number_is_power_of_two_v<N>)
				return static_cast<value_type>(_acc >> lb<N>);
			return static_cast<value_type>(_acc / N);
		}

	private:
		value_type _samples[N]{};                   /**< Буфер для хранения последних N выборок. */
		core::make_wider_type_t<value_type> _acc{}; /**< Расширенный тип для предотвращения переполнения при накоплении суммы. */
		pos_type _head{};                           /**< Индекс следующей позиции для записи. */
		bool _full{};                               /**< Флаг, обозначающий, что окно заполнено. */
	};

} /// !namespace math

#endif /// !defined(SMA_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)

