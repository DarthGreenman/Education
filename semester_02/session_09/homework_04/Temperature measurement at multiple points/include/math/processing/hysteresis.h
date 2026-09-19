/// hysteresis.h

#ifndef HYSTERESIS_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define HYSTERESIS_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <common.h>
#include <type_traits.h>

#if defined(_MSC_VER)
#include <cmath>
#elif defined(__GNUC__) || defined(__clang__)
#include <math.h>
#endif

namespace math {

	/**********************************************************************************************
	 * @brief Политика гистерезиса.
	 *
	 * Предназначена для подавления небольших колебаний входного сигнала. Новое значение принимается
	 * только в том случае, если его отличие от текущего опорного значения не меньше заданного
	 * порога Threshold. После принятия нового значения, оно становится новым опорным значением для
	 * последующих сравнений.
	 *
	 * @attention Гистерезис, как впрочем и любая другая вычислительная структура с поведением
	 *            детектора, не имеет осмысленного состояния до получения первого sample. Первый sample
	 *            выполняет ленивую инициализацию внутреннего состояния и не считается событием.
	 *
	 * Жизненный цикл объекта:
	 *       1. Ленивая инициализация внутреннего состояния при первом вызове update(...)
	 *       2. update(...)
	 *       3. value()
	 *
	 * @tparam Threshold Пороговое значение, определяющее минимальное изменение сигнала, которое будет
	 *                   принято как новое значение.
	 */
	template <typename T, auto Threshold = 3>
	struct hysteresis {
		using value_type = T;
		static_assert(core::is_arithmetic_integer_v<value_type>, "Only integer types are supported.");
		static_assert(core::is_arithmetic_integer_v<decltype(Threshold)>, "Only integer types are supported.");
		static_assert(Threshold > 0, "Threshold must be greater than zero.");

		/******************************************************************************************
		 * @brief Обновляет внутреннее состояние гистерезиса новым измерением.
		 *
		 * Если разница между новым измерением и текущим опорным значением превышает порог Threshold,
		 * то новое измерение принимается как текущее значение, и функция возвращает true. В противном
		 * случае текущее значение остаётся неизменным, и функция возвращает false.
		 * @param[in] sample Новое измерение.
		 * @return true, если новое измерение принято как текущее значение; false в противном случае.
		 */
		[[nodiscard]] FORCEINLINE bool update(value_type sample) noexcept {
			if(!_initialized) {
				_reference_value = sample;
				_initialized = true;
				return false;
			}

			if(const auto difference = abs(sample - _reference_value);
				difference >= Threshold) {
				_reference_value = sample;
				return true;
			}
			return false;
		}

		/******************************************************************************************
		 * @brief Возвращает текущее принятое значение.
		 *
		 * Значение остаётся неизменным до тех пор, пока очередное измерение не превысит порог
		 * гистерезиса.
		 * @return Текущее принятое значение.
		 */
		[[nodiscard]] FORCEINLINE value_type value() const noexcept { return _reference_value; }

	private:
		/* Текущее опорное значение.  Именно относительно этого значения вычисляется отклонение
		 * каждого нового измерения. После успешного обновления становится новой точкой отсчёта. */
		value_type _reference_value{};
		bool _initialized{}; /// Флаг, обозначающий, что интегратор был инициализирован первым sample.
	};

} /// !namespace math

#endif /// !defined(HYSTERESIS_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)
