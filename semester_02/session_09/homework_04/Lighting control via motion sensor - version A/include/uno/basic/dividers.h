/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * brief Делитель напряжения.
 *
 */
/*
 * Сущности:
 * enum class placement - положение неизвестного сопротивления в делителе напряжения.
 *
 * placement::up:
 *
 *       Vcc
 *        │
 *       Rt
 *        │
 *        ├── Vout
 *        │
 *       R0
 *        │
 *       GND
 *
 * placement::down:
 *
 *       Vcc
 *        │
 *       R0
 *        │
 *        ├── Vout
 *        │
 *       Rt
 *        │
 *       GND
 *
 * Здесь Rt — неизвестное сопротивление, а R0 — известное сопротивление.
 * 
 * template<auto... Rs> struct voltage_divider - делитель напряжения.
 * Вычисляет нормированное сопротивление неизвестного резистора по результату
 * измерения АЦП.
 * Результатом работы является не сопротивление в Омах, а отношение:
 *
 *                  Rt
 *             r = ────
 *                  Rfix
 *
 * Результат является безразмерной величиной. Такое представление позволяет 
 * отделить электрическую часть измерения от последующего алгоритма вычисления
 * температуры.
 * 
 * Последовательность вычисления: 
 *            ADC → voltage_divider → Rt / R0 → temperature algorithm
 * 
 * Для placement::up:
 *
 *        Vcc
 *         │
 *        Rt
 *         │
 *         ├── Vout
 *         │
 *        Rfix
 *         │
 *        GND
 *
 * Из закона делителя:
 *
 *              Vout       Rfix
 *              ──── = ────────────
 *              Vcc     Rt + Rfix
 *
 * Отсюда:
 *
 *               Rt       Vcc - Vout
 *              ──── = ────────────────
 *               Rfix       Vout
 *
 * Так как:
 *
 *              Vout        signal
 *              ──── = ────────────────
 *              Vcc     maximum_signal
 *
 * Следовательно:
 *
 *               Rt     maximum_signal - signal
 *              ──── = ─────────────────────────
 *               Rfix          signal
 *
 *
 * Для placement::down:
 *
 *        Vcc
 *         │
 *        Rfix
 *         │
 *         ├── Vout
 *         │
 *        Rt
 *         │
 *        GND
 *
 * Из закона делителя:
 *
 *              Vout        Rt
 *              ──── = ────────────
 *              Vcc     Rfix + Rt
 *
 * Отсюда:
 *
 *               Rt          Vout
 *              ──── = ────────────────
 *               Rfix     Vcc - Vout
 *
 * Следовательно:
 *
 *               Rt           signal
 *              ──── = ───────────────────────
 *               Rfix  maximum_signal - signal
 *
 * Таким образом, метод выполняет только преобразование результата АЦП в 
 * нормированное сопротивление.
 *
 * Граничные значения:
 *
 *     signal == 0
 *     signal == maximum_signal
 *
 * соответствуют предельным значениям сопротивления идеальной математической 
 * модели (нулевому или бесконечному сопротивлению в зависимости от положения Rt).
 *
 * Поэтому они не входят в область определения данного метода. На уровне системы 
 * такие значения должны быть обработаны hardware_fault_detection до вызова 
 * voltage_divider.
 *
 */

#ifndef DIVIDERS_H_FOR_THE_ARDUINO_UNO_PROJECT
#define DIVIDERS_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <common.h>
#include <exp.h>
#include <type_traits.h>

namespace uno {
	enum class placement {
		up,
		down
	};

	template<auto... Rs>
	struct voltage_divider;
	/*
	 * @tparam P положение неизвестного сопротивления в делителе
	 * @tparam B разрядность АЦП
	 */
	template<placement P, auto B>
	struct voltage_divider<P, B> {
		static_assert(core::is_arithmetic_integer_v<decltype(B)>, "Only integer types are supported.");
		static_assert(B > 0, "Bit depth must be positive.");
		/*
		 * Максимальный код (maximum_signal) АЦП для заданной разрядности.
		 * Для N-разрядного АЦП: U = 2^N - 1, например:
		 *
		 *     10 бит → 1023
		 *     12 бит → 4095
		 *     16 бит → 65535
		 */
		static constexpr auto maximum_signal = math::exp_v<2, B> -1;
		/**
		 * @brief Вычисляет нормированное сопротивление Rt / Rfix.
		 *
		 * Предусловие: 0 < signal < maximum_signal
		 *
		 * Проверка этого предусловия выполняется до вызова метода. 
		 * Сам voltage_divider не занимается диагностикой неисправностей, 
		 * не изменяет входное значение и не пытается корректировать 
		 * граничные значения.
		 *
		 * @param [in] signal результат преобразования АЦП
		 * @return            нормированное сопротивление Rt / Rfix.
		 */
		template<typename T>
		[[nodiscard]] FORCEINLINE float operator()(T signal) const noexcept {
			static_assert(core::is_arithmetic_integer_v<T>, "Only integer types are supported.");

			if constexpr(P == placement::up)
				return (static_cast<float>(maximum_signal) - signal) / signal;
			else
				return static_cast<float>(signal) / (maximum_signal - signal);
		}
	};

} /// !namespace uno

#endif // !defined(DIVIDERS_H_FOR_THE_ARDUINO_UNO_PROJECT)

