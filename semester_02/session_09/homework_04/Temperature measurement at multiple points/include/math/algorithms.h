/// algorithms.h

#ifndef ALGORITHMS_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define ALGORITHMS_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <abs.h>
#include <common.h>
#include <log.h>
#include <numeric_limits.h>
#include <type_traits.h>
#include <utility.h>
#include <view_interfaces.h>

#if defined(_MSC_VER)
#include <cmath>
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <math.h>
#include <stdint.h>
#endif
#include <meta.h>

namespace math {

	/**********************************************************************************************
	 * @brief Декомпозирует 32-битное целое число в массив элементов типа T.
	 *
	 * Шаблон принимает 32-битное целое число и декомпозирует его на массив элементов типа T, используя
	 * битовые смещения, указанные в параметрах шаблона. Каждый элемент массива получается путем сдвига
	 * числа на соответствующее количество бит и применения маски для извлечения нужного значения.
	 * Это может быть полезно для создания шаблонов классов и функций, которые требуют декомпозиции
	 * чисел на отдельные элементы на этапе компиляции, например, для оптимизации алгоритмов.
	 *
	 * @tparam T     Тип элемента массива, который будет возвращен, в данном случае это может быть
	 *               uint8_t, uint16_t или любой другой тип, который может хранить значения,
	 *               извлеченные из 32-битного числа.
	 * @tparam N     Количество битовых смещений, которые будут использоваться для декомпозиции числа,
	 *               оно зависит от типа T.
	 *               Значение каждого B указывает, на сколько бит нужно сдвинуть число, чтобы получить
	 *               соответствующий элемент массива.
	 * @param value  32-битное целое число, которое будет декомпозировано.
	 * @param mask   Маска, которая будет использоваться для извлечения значений из числа.
	 *
	 * @return       Возвращает обертку для массива элементов типа T, содержащую декомпозированные
	 *               значения.
	 */
	template <typename T, uint64_t... N,
		typename = core::enable_if_t<core::is_integral_v<T>&& core::is_unsigned_v<T>>>
	[[nodiscard]] FORCEINLINE constexpr auto decompose(uint32_t value, uint8_t mask) noexcept {
		static_assert((((!(N % 1) && core::is_same_v<T, uint8_t>) || (!(N % 4) && core::is_same_v<T, uint16_t>)) && ...),
			"The offset (N) for uint8_t type values must be a multiple of one, and for uint16_t types "
			"it must be a multiple of four.");
		static_assert(((N <= core::numeric_limits<T>::width) && ...), "Each N must be < number of bits in T.");

		auto decomposing = [&](uint8_t shift) { return static_cast<T>((value >> shift) & mask); };
		return core::wrapper_for_array<T, sizeof...(N)>{ decomposing(N)... };
	}

	template<typename T>
	struct calculate_fraction {
	private:
		static_assert(has_static_antecedent_v<T>, "T must provide static constexpr antecedent.");
		static_assert(has_static_consequent_v<T>, "T must provide static constexpr consequent.");
		static constexpr auto num = abs_v<T::antecedent>;
		static constexpr auto den = T::consequent;
	public:
		static constexpr auto value = []() noexcept {
			//core::pure_type_t<decltype(num)> fraction{};
			//while(num * exp(2, fraction) < den) { ++fraction; }
			return lb<num> -lb<den>;
		}();
	};

	/**********************************************************************************************
	 * @brief Шаблон алгоритма вычисления температуры.
	 *
	 * Специализации данного шаблона реализуют различные математические модели,
	 * связывающие измеренное сопротивление термистора с его температурой.
	 *
	 * Входной величиной температурного алгоритма является нормированное сопротивление:
	 *
	 *                         Rt
	 *                  ratio = ──
	 *                         R0
	 *
	 * где:
	 *     Rt — текущее сопротивление термистора;
	 *     R0 — сопротивление термистора при опорной температуре T0.
	 *
	 * Нормированное сопротивление является безразмерной величиной и позволяет
	 * отделить электрическую часть измерения от конкретной модели термистора.
	 *
	 * @tparam Cs Пакет коэффициентов, определяющих используемую математическую модель.
	 */
	template<auto... Cs>
	struct temperature_calculation;

	/**********************************************************************************************
	 * @brief Специализация для расчёта температуры по уравнению Стейнхарта-Харта.
	 *
	 * Классическая форма уравнения:
	 *
	 *                  1
	 *                  ── = A + B·ln(Rt) + C·ln³(Rt)
	 *                  T
	 *
	 * Для работы непосредственно с нормированным сопротивлением ratio = Rt / R0
	 * уравнение может быть преобразовано соответствующим образом.
	 *
	 * @tparam A Коэффициент A.
	 * @tparam B Коэффициент B.
	 * @tparam C Коэффициент C.
	 */
	template<auto A, auto B, auto C>
	struct temperature_calculation<A, B, C> {}; /// Расчет по уравнению Стейнхарта-Харта

	/**********************************************************************************************
	 * @brief Специализация для расчёта температуры по Beta equation.
	 *
	 * Используется уравнение:
	 *
	 *                    1       1       1
	 *                    ── =    ─── +   ── · ln(Rt / R0)
	 *                    T       T0      B
	 *
	 * где:
	 *
	 *     T  — абсолютная температура в Кельвинах;
	 *     T0 — абсолютная опорная температура в Кельвинах;
	 *     B  — Beta coefficient термистора;
	 *     Rt — текущее сопротивление термистора;
	 *     R0 — сопротивление термистора при температуре T0.
	 *
	 * Поскольку voltage_divider уже вычисляет нормированное сопротивление:
	 *
	 *                    Rt
	 *             ratio = ──
	 *                    R0
	 *
	 * температурный алгоритм непосредственно принимает ratio.
	 *
	 * Последовательность вычислений:
	 *
	 *     1. ratio = Rt / R0
	 *     2. L = ln(ratio)
	 *     3. inverse_temperature = 1 / T0 + L / B
	 *     4. T = 1 / inverse_temperature
	 *     5. temperature_C = T - 273.15
	 *
	 * @tparam B  Beta coefficient конкретного термистора.
	 * @tparam T0 Опорная температура в градусах Цельсия.
	 *
	 * @note
	 * В технической документации термистора коэффициент B обычно указывается
	 * в Кельвинах. Опорная температура T0 задаётся в градусах Цельсия только
	 * для удобства конфигурации и перед вычислением переводится в Кельвины.
	 */
	template<auto B>
	struct temperature_calculation<B> {
		static_assert(core::is_arithmetic_integer_v<decltype(B)>, "Only integer types are supported.");
		static_assert(B > 0, "The Beta coefficient should at least be positive, and moreover "
			"correspond to the value specified in the technical description of the element.");

		[[nodiscard]] FORCEINLINE auto operator()(float ratio) const noexcept {
			static constexpr float reference_temperature{ 25.00f }; /// Опорная температура R0 для Beta-модели, °C.
			static constexpr float kelvin_offset{ 273.15f };        /// 0 град. Цельсия = 273.15 град. Кельвина
			return 1.0f /
				(1.0f /
					(reference_temperature + kelvin_offset) +
					logf(ratio) / B
				) -
				kelvin_offset;
		}
	}; /// Расчет по модифицированному уравнению

} /// !namespace math

#endif /// !defined(ALGORITHMS_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)

