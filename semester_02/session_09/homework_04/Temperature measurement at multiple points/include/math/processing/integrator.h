/// integrator.h

#ifndef INTERGATOR_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define INTERGATOR_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <common.h>
#include <type_traits.h>

namespace math {

	/**********************************************************************************************
	 * @brief Политика интегратора.
	 *
	 * Предназначен для подавления случайных выбросов и дребезга входного сигнала.
	 * Инвариант: после назначения нового кандидата уровень доверия всегда равен Threshold. Пока
	 * доверие не исчерпано, кандидат не изменяется. Смена кандидата происходит только после
	 * полной утраты доверия к предыдущему кандидату, после чего новый кандидат сразу получает
	 * полный уровень доверия.
	 *
	 * @attention Интегратор, как впрочем и любая другая вычислительная структура с поведением
	 *            детектора, не имеет осмысленного состояния до получения первого sample. Первый sample
	 *            выполняет ленивую инициализацию внутреннего состояния и не считается событием.
	 *
	 * Жизненный цикл объекта:
	 *       1. Ленивая инициализация внутреннего состояния при первом вызове update(...)
	 *       2. update(...)
	 *       3. value()
	 *
	 * @tparam T         Тип обрабатываемых значений.
	 * @tparam Threshold Пороговое значение, определяющее количество последовательных совпадений,
	 *                   необходимых для подтверждения нового значения.
	 */
	template <typename T, auto Threshold = 3>
	struct integrator {
		using value_type = T;
		static_assert(core::is_arithmetic_integer_v<value_type>, "Only integer types are supported.");
		static_assert(core::is_arithmetic_integer_v<decltype(Threshold)>, "Only integer types are supported.");
		static_assert(Threshold > 0, "Limit must be greater than zero.");

		/******************************************************************************************
		 * @brief Обрабатывает новое измерение.
		 *
		 * Инвариант: после назначения нового кандидата уровень доверия всегда равен Threshold. Пока
		 * доверие не исчерпано, кандидат не изменяется. Смена кандидата происходит только после
		 * полной утраты доверия к предыдущему кандидату, после чего новый кандидат сразу получает
		 * полный уровень доверия.
		 * @param[in] sample Новое измерение.
		 * @return true, если подтвержденное значение изменилось; false, если изменение проигнорировано.
		 */
		[[nodiscard]] FORCEINLINE bool update(value_type sample) noexcept {
			/// Первая инициализация
			if(!_initialized) {
				_candidate_value = sample;
				_confidence = Threshold;
				_initialized = true;
				return false;
			}
			/// Обновление доверия к кандидату
			if(sample == _candidate_value) {
				if(_confidence < Threshold)
					++_confidence;
			} else {
				if(_confidence > 0)
					--_confidence;
			}
			/// Подтверждение нового значения
			if(_confidence == Threshold && _confirmed_value != _candidate_value) {
				_confirmed_value = _candidate_value;
				return true;
			}
			/// Смена кандидата
			if(_confidence == 0) {
				_candidate_value = sample;
				_confidence = Threshold;
				return false;
			}
			/// Подтвержденное значение не изменилось
			return false;
		}

		/******************************************************************************************
		 * @brief Возвращает текущее подтвержденное значение.
		 *
		 * Значение остаётся неизменным до тех пор, пока очередное измерение не совпадет с кандидатом
		 * Threshold раз подряд.
		 * @return Текущее подтвержденное значение.
		 */
		[[nodiscard]] FORCEINLINE value_type value() const noexcept { return _confirmed_value; }

	private:
		value_type _confirmed_value{}; /// Подтвержденное значение, к которому полное доверие.
		value_type _candidate_value{}; /// Текущий кандидат, значение которого сравнивается с новым sample.
		decltype(Threshold) _confidence{}; /// Уровень доверия к кандидату.
		bool _initialized{}; /// Флаг, обозначающий, что интегратор был инициализирован первым sample.
	};

} /// !namespace math

#endif /// !defined(INTERGATOR_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)

