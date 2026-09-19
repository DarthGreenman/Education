/// log.h

#ifndef LOGARITHM_H_THE_COMPONENT_LIBRARY_PROJECT
#define LOGARITHM_H_THE_COMPONENT_LIBRARY_PROJECT

#include <type_traits.h>

namespace math {

	namespace helper {
		template<auto B, auto N>
		struct log {
			static_assert(core::is_arithmetic_integer_v<decltype(B)>, "Base must be an integer type.");
			static_assert(core::is_arithmetic_integer_v<decltype(N)>, "Value must be an integer type.");
			static_assert(B > 1, "Base must be greater than one.");
			static_assert(N > 0, "Value must be greater than zero.");
			/**************************************************************************************
			 * @brief Целая часть логарифма.
			 *
			 * Алгоритм последовательно делит значение на основание логарифма, подсчитывая количество
			 * выполненных делений. Полученный результат соответствует
			 *
			 *      floor(log_B(B)).
			 */
			static constexpr auto value = []() noexcept {
				decltype(N) exp{};
				auto num = N;
				while(num > 1) {
					num /= B;
					++exp;
				}
				return exp;
			}();
		};

	} /// !namespace helper

	/**********************************************************************************************
	 * @brief Вычисляет целую часть логарифма целочисленной константы.
	 *
	 * Метафункция вычисляет
	 *
	 *      floor(log_B(N))
	 *
	 * где:
	 *  - B — основание логарифма;
	 *  - N — положительное целое число.
	 *
	 * Вычисление производится последовательным делением значения на основание логарифма до тех пор,
	 * пока результат не станет меньше либо равен единице. Количество выполненных делений соответствует
	 * целой части логарифма.
	 *
	 * @tparam B Основание логарифма. Должно быть больше единицы.
	 * @tparam N Значение, для которого вычисляется логарифм.
	 */
	template<auto B, auto N>
	inline constexpr auto log_v = helper::log<B, N>::value;

	/**********************************************************************************************
	 * @brief Целая часть двоичного логарифма.
	 *
	 * Эквивалентно: log_v<2, N>
	 *
	 * @tparam N Положительное целое число.
	 */
	template<auto N>
	inline constexpr auto lb = log_v<2, N>;

	/**********************************************************************************************
	 * @brief Целая часть десятичного логарифма.
	 *
	 * Эквивалентно: log_v<10, N>
	 *
	 * @tparam N Положительное целое число.
	 */
	template<auto N>
	inline constexpr auto lg = log_v<10, N>;



} /// !namespace math

#endif /// !defined(LOGARITHM_H_THE_COMPONENT_LIBRARY_PROJECT)
