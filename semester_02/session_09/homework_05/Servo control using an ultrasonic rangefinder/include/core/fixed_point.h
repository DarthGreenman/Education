/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Число с фиксированной двоичной точкой.
 *
 * Представление основано на целочисленном хранилище с фиксированным
 * количеством бит, выделенных под дробную часть.
 *
 * Гарантии данной реализации:
 * - число с фиксированной точкой может быть сконструировано из числа
 *   с плавающей точкой и целого числа без потери точности в пределах
 *   представимого диапазона;
 * - предоставляет диапазон значений согласно выбранной ширине внутреннего
 *   представления и количеству бит, выделенных под дробную часть;
 * - число может быть восстановлено в число с плавающей точкой;
 * - сохраняет свою точность в пределах представимого диапазона;
 * - фиксированный размер внутреннего представления числа обеспечивает
 *   предсказуемое поведение при арифметических операциях;
 * - коммутативность и ассоциативность операций сложения и умножения сохраняются
 *   в пределах представимого диапазона;
 * - контроль переполнения осуществляется с использованием политик обработки
 *   результатов арифметических операций;
 */
 /*
  * Абсолютная неточность: В бытовой или технической речи фраза «нулевая точность»
  * означает полное отсутствие точности (то есть стопроцентную погрешность), когда
  * результат вычислений или измерений абсолютно неверен и бесполезен.
 */

#ifndef FIXED_POINT_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define FIXED_POINT_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include "common.h"
#include <numeric_limits.h>
#include <suitable_type.h>
#include <wider_type.h>
#include <type_traits.h>
#include <saturation.h>
#include <meta.h>
#include "utility.h"

#if defined(_MSC_VER)
#include <cassert>
#elif defined(__GNUC__) || defined(__clang__)
#include <assert.h>
#endif

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace core {
	/**
	 * @brief Определяет знаковость внутреннего представления числа.
	 */
	enum class signedness : bool {
		unsigned_type,
		signed_type
	};

	/**
	 * @brief Число с фиксированной двоичной точкой.
	 *
	 * @cond HIDE_ENUM_BRIEF
	 * value_type         определяет тип внутреннего представления
	 * wider_type         ближайший более широкий целочисленный тип
	 *                    с знаковостью value_type
	 * wider_signed_type  wider_type для знакового value_type либо
	 *                    его знаковый аналог для беззнакового value_type
	 *
	 * Диапазон wider_signed_type должен быть достаточен для промежуточных
	 * результатов каждой поддерживаемой операции. Это проверяется отдельно
	 * для каждой операции, а не предполагается автоматически из того,
	 * что тип шире.
	 *
	 * Реальное значение определяется выражением:
	 * value = representation / 2^Fraction.
	 * @endcond
	 *
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 */
	template<
		unsigned short int Width,
		unsigned short int Fraction,
		signedness Signed = signedness::signed_type,
		typename BehaviorPolicy = saturation_policy
	>
	class fixed_point {
		static_assert(Width == 8 || Width == 16 || Width == 32, "Width must be "
			"exactly 8, 16, or 32 bits for maximum CPU speed.");
		static_assert(Fraction < Width, "Fractional part must be smaller than "
			"the total Width.");
	public:
		using value_type = make_suitable_type_t<Width, static_cast<bool>(Signed)>;
		using policy_type = BehaviorPolicy;

		constexpr fixed_point() = default;
		explicit constexpr fixed_point(float number) noexcept {
			if constexpr(Signed == signedness::unsigned_type) {
				assert(number >= 0.0f && "Unsigned fixed_point cannot be negative");
			}
			_rep = policy_type::template apply<value_type>(
				static_cast<make_wider_type_t<value_type>>(number * scale)
			);
		}
		template<typename T, typename = enable_if_t<is_arithmetic_integer_v<T>>>
		constexpr fixed_point(T number) noexcept
			: _rep{ policy_type::template apply<value_type>(move_point_left(
				static_cast<make_wider_type_t<value_type>>(number)
			)) } {}

		fixed_point(const fixed_point&) = default;
		fixed_point(fixed_point&&) = default;
		~fixed_point() = default;

		fixed_point& operator=(const fixed_point&) = default;
		fixed_point& operator=(fixed_point&&) = default;

		/**
		 * @brief Восстанавливает реальное значение.
		 * @return значение с плавающей точкой, восстановленное из
		 *         внутреннего представления.
		 */
		[[nodiscard]] FORCEINLINE constexpr auto restore() const
			noexcept { return static_cast<float>(_rep) / scale; }

		/**
		 * @brief Предоставляет минимальное значение числа с
		 *        фиксированной точкой.
		 * @return минимальное значение типа fixed_point.
		 */
		[[nodiscard]] FORCEINLINE static constexpr auto min()
			noexcept {
			return static_cast<float>(numeric_limits<value_type>::min()) / scale;
		}

		/**
		 * @brief Предоставляет максимальное значение числа с
		 *        фиксированной точкой.
		 * @return максимальное значение типа fixed_point.
		 */
		[[nodiscard]] FORCEINLINE static constexpr auto max()
			noexcept {
			return static_cast<float>(numeric_limits<value_type>::max()) / scale;
		}

		/**
		 * @brief Перегруженный оператор += для сложения двух чисел с
		 *        фиксированной точкой.
		 *
		 * @param [in] rhs число с фиксированной точкой, которое нужно добавить
		 *                 к текущему объекту
		 * @return         ссылка на текущий объект после выполнения операции
		 *                 сложения
		 */
		FORCEINLINE constexpr fixed_point& operator+=(const fixed_point& rhs)
			noexcept {
			using calculations_type = make_wider_type_t<value_type>;
			const auto tmp = static_cast<calculations_type>(_rep) +
				static_cast<calculations_type>(rhs._rep);

			_rep = policy_type::template apply<value_type>(tmp);
			return *this;
		}

		/**
		 * @brief Перегруженный оператор -= для вычитания двух чисел с
		 *        фиксированной точкой.
		 *
		 * @param [in] rhs число с фиксированной точкой, которое нужно вычесть
		 *                 из текущего объекта
		 * @return         ссылка на текущий объект после выполнения операции
		 *                 вычитания
		 */
		FORCEINLINE constexpr fixed_point& operator-=(const fixed_point& rhs)
			noexcept {
			using calculations_type = make_signed_t<make_wider_type_t<value_type>>;
			const auto tmp = static_cast<calculations_type>(_rep) -
				static_cast<calculations_type>(rhs._rep);

			_rep = policy_type::template apply<value_type>(tmp);
			return *this;
		}

		/**
		 * @brief Перегруженный оператор *= для умножения двух чисел с
		 *        фиксированной точкой.
		 *
		 * Представления обоих операндов преобразуются к расширенному типу,
		 * после чего вычисляется их полное произведение. Результат
		 * масштабируется с отбрасыванием дробной части и округлением к нулю.
		 * Затем к нему применяется политика обработки диапазона целевого типа.
		 * Если оба операнда имеют F дробных битов, а их целочисленные
		 * представления равны a и b, представление результата до применения
		 * политики определяется выражением:
		 *
		 * @param [in] rhs число с фиксированной точкой, которое нужно умножить
		 *                 на текущий объект
		 * @return         ссылка на текущий объект после выполнения операции
		 *                 умножения
		 */
		FORCEINLINE constexpr fixed_point& operator*=(const fixed_point& rhs)
			noexcept {
			using calculations_type = make_wider_type_t<value_type>;
			const auto tmp = move_point_right(static_cast<calculations_type>(_rep) *
					static_cast<calculations_type>(rhs._rep));

			_rep = policy_type::template apply<value_type>(tmp);
			return *this;
		}

		/**
		 * @brief Перегруженный оператор /= для деления двух чисел с
		 *        фиксированной точкой.
		 *
		 * @param [in] rhs число с фиксированной точкой, на которое нужно
		 *                 разделить текущий объект
		 * @return         ссылка на текущий объект после выполнения операции
		 *                 деления
		 */
		FORCEINLINE constexpr fixed_point& operator/=(const fixed_point& rhs)
			noexcept {
			assert(rhs._rep != 0 && "Division by zero.");
			using calculations_type = make_wider_type_t<value_type>;
			const auto tmp = move_point_left(static_cast<calculations_type>(_rep)) /
				static_cast<calculations_type>(rhs._rep);

			_rep = policy_type::template apply<value_type>(tmp);
			return *this;
		}

		/* Перегруженный оператор сравнения < для двух чисел с фиксированной точкой. */
		friend FORCEINLINE constexpr bool operator<(const fixed_point& lhs, const fixed_point& rhs)
			noexcept { return lhs._rep < rhs._rep; }

		/* Перегруженный оператор сравнения > для двух чисел с фиксированной точкой. */
		friend FORCEINLINE constexpr bool operator>(const fixed_point& lhs, const fixed_point& rhs)
			noexcept { return !(lhs < rhs); }

		/* Перегруженный оператор сравнения == для двух чисел с фиксированной точкой. */
		friend FORCEINLINE constexpr bool operator==(const fixed_point& lhs, const fixed_point& rhs)
			noexcept { return lhs._rep == rhs._rep; }

		/* Перегруженный оператор сравнения != для двух чисел с фиксированной точкой. */
		friend FORCEINLINE constexpr bool operator!=(const fixed_point& lhs, const fixed_point& rhs)
			noexcept { return !(lhs == rhs); }

		/* Перегруженный оператор сравнения <= для двух чисел с фиксированной точкой. */
		friend FORCEINLINE constexpr bool operator<=(const fixed_point& lhs, const fixed_point& rhs)
			noexcept { return lhs < rhs || lhs == rhs; }

		/* Перегруженный оператор сравнения >= для двух чисел с фиксированной точкой. */
		friend FORCEINLINE constexpr bool operator>=(const fixed_point& lhs, const fixed_point& rhs)
			noexcept { return lhs > rhs || lhs == rhs; }

	private:
		/**
		 * @brief Перемещает двоичную точку влево на количество бит, определенное
		 *        параметром Fraction.
		 *
		 * Используется при операциях умножения иделения целочисленных
		 * представлений чисел с фиксированной точкой.
		 *
		 * Сдвиг влево << для отрицательного знакового целого — это UB. То есть
		 * компилятор вправе выдать любой результат, оптимизировать код
		 * непредсказуемо или даже «сломать» логику программы.
		 *
		 * @param [in] number число, у которого нужно переместить двоичную точку
		 * @return            число с перемещенной двоичной точкой влево
		 */
		template<typename T, typename = enable_if_t<is_arithmetic_integer_v<T>>>
		[[nodiscard]] FORCEINLINE static constexpr auto move_point_left(T number)
			noexcept {
			if constexpr(is_signed_v<T>) {
				if(is_negative(number)) {
					using U = make_unsigned_t<T>;
					const U magnitude = U{} - static_cast<U>(number);
					/// Обрабатывает граничные условия
					if(const U shifted = magnitude >> Fraction;
						shifted > static_cast<U>(numeric_limits<T>::max())) {
						return numeric_limits<T>::min();
					} else { return static_cast<T>(-static_cast<T>(shifted)); }
				} else { return static_cast<T>(number << Fraction); }
			} else { return static_cast<T>(number << Fraction); }
		}

		/**
		 * @brief Перемещает двоичную точку вправо на количество бит, определенное
		 *        параметром Fraction.
		 *
		 * Используется при операциях умножения иделения целочисленных
		 * представлений чисел с фиксированной точкой.
		 *
		 * @param [in] number число, у которого нужно переместить двоичную точку
		 * @return            число с перемещенной двоичной точкой вправо
		 */
		template<typename T, typename = enable_if_t<is_arithmetic_integer_v<T>>>
		[[nodiscard]] FORCEINLINE static constexpr auto move_point_right(T number)
			noexcept {
			if constexpr(is_signed_v<T>) {
				if(is_negative(number)) {
					using U = make_unsigned_t<T>;
					const U magnitude = U{} - static_cast<U>(number);
					/// Обрабатывает граничные условия 
					if(const U shifted = magnitude << Fraction;
						shifted > static_cast<U>(numeric_limits<T>::max())) {
						return numeric_limits<T>::min();
					} else { return static_cast<T>(-static_cast<T>(shifted)); }
				} else { return static_cast<T>(number >> Fraction); }
			} else { return static_cast<T>(number >> Fraction); }
		}

	private:
		static constexpr auto scale =
			static_cast<make_wider_type_t<value_type>>(1U) << Fraction; /**< Масштаб */
		value_type _rep{}; /**< Внутреннее представление числа с фиксированной точкой. */
	};
	/*************************************************************************/
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy
	>
	FORCEINLINE constexpr auto operator+(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		fixed_point<Width, Fraction, Signed, BehaviorPolicy> rhs) noexcept {
		return lhs += rhs;
	}
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy
	>
	FORCEINLINE constexpr auto operator+(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		float rhs) noexcept {
		return lhs += fixed_point<Width, Fraction, Signed, BehaviorPolicy>{rhs};
	}
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy, typename T
	>
	FORCEINLINE constexpr auto operator+(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		T rhs) noexcept {
		static_assert(is_arithmetic_integer_v<T>, "Only integer types are supported.");
		return lhs += fixed_point<Width, Fraction, Signed, BehaviorPolicy>{rhs};
	}
	/*************************************************************************/
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy
	>
	FORCEINLINE constexpr auto operator-(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		fixed_point<Width, Fraction, Signed, BehaviorPolicy> rhs) noexcept {
		return lhs -= rhs;
	}
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy
	>
	FORCEINLINE constexpr auto operator-(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		float rhs) noexcept {
		return lhs -= fixed_point<Width, Fraction, Signed, BehaviorPolicy>{rhs};
	}
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @tparam T
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy, typename T
	>
	FORCEINLINE constexpr auto operator-(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		T rhs) noexcept {
		static_assert(is_arithmetic_integer_v<T>, "Only integer types are supported.");
		return lhs -= fixed_point<Width, Fraction, Signed, BehaviorPolicy>{rhs};
	}
	/*************************************************************************/
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy
	>
	FORCEINLINE constexpr auto operator*(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		fixed_point<Width, Fraction, Signed, BehaviorPolicy> rhs) noexcept {
		return lhs *= rhs;
	}
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy
	>
	FORCEINLINE constexpr auto operator*(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		float rhs) noexcept {
		return lhs *= fixed_point<Width, Fraction, Signed, BehaviorPolicy>{rhs};
	}
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @tparam T
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy, typename T
	>
	FORCEINLINE constexpr auto operator*(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		T rhs) noexcept {
		static_assert(is_arithmetic_integer_v<T>, "Only integer types are supported.");
		return lhs *= fixed_point<Width, Fraction, Signed, BehaviorPolicy>{rhs};
	}
	/*************************************************************************/
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy
	>
	FORCEINLINE constexpr auto operator/(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		fixed_point<Width, Fraction, Signed, BehaviorPolicy> rhs) noexcept {
		return lhs /= rhs;
	}
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy
	>
	FORCEINLINE constexpr auto operator/(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		float rhs) noexcept {
		return lhs /= fixed_point<Width, Fraction, Signed, BehaviorPolicy>{rhs};
	}
	/**
	 * @brief
	 * @tparam Width          количество бит внутреннего представления
	 * @tparam Fraction       количество бит дробной части, определяющее
	 *                        положение двоичной точки
	 * @tparam Signed         знаковость внутреннего представления
	 * @tparam BehaviorPolicy политика обработки результатов арифметических
	 *                        операций, выходящих за пределы представимого
	 *                        диапазона
	 * @tparam T
	 * @param lhs
	 * @param rhs
	 * @return
	 */
	template<
		unsigned short int Width, unsigned short int Fraction, signedness Signed,
		typename BehaviorPolicy, typename T
	>
	FORCEINLINE constexpr auto operator/(fixed_point<Width, Fraction, Signed, BehaviorPolicy> lhs,
		T rhs) noexcept {
		static_assert(is_arithmetic_integer_v<T>, "Only integer types are supported.");
		return lhs /= fixed_point<Width, Fraction, Signed, BehaviorPolicy>{rhs};
	}

} /// !namespace core

#pragma pop_macro("min")
#pragma pop_macro("max")

#endif /// !defined(FIXED_POINT_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)

