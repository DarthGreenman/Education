/// fixed_point.h

/**************************************************************************************************
 Пример: Q7.8

 S IIIIIII FFFFFFFF
 │ │       │
 │ │       └── 8 бит дробной части Fraction
 │ └────────── 7 бит целой части   Integer
 └──────────── знак

 Архитектура класса:
 fixed_point
 |
 ├── представление
 │      Width
 │      Fraction
 │      Signed
 │
 └── семантика
		BehaviorPolicy

**************************************************************************************************/

#ifndef FIXED_POINT_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define FIXED_POINT_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <common.h>
#include <numeric_limits.h>
#include <type_traits.h>
#include <wider_type.h>
#include <wraparound.h>
#include "meta.h"

namespace core {


	/**********************************************************************************************
	 * @brief Число с фиксированной двоичной точкой.
	 *
	 * Представление основано на целочисленном хранилище с фиксированным
	 * количеством бит, выделенных под дробную часть.
	 *
	 * Реальное значение вычисляется как:
	 *
	 * value = representation / 2^Fraction
	 *
	 * @tparam Width
	 *         Полная ширина представления в битах.
	 *
	 * @tparam Fraction
	 *         Количество бит, используемых для хранения дробной части.
	 *         Определяет положение двоичной точки.
	 *
	 * @tparam Signed
	 *         Признак знакового представления.
	 *         true  - используется знаковый тип;
	 *         false - используется беззнаковый тип.
	 *
	 * @tparam BehaviorPolicy
	 *         Политика арифметического поведения.
	 *
	 *         Определяет обработку результатов операций, которые
	 *         не могут быть представлены внутренним представлением.
	 *
	 *         Примеры:
	 *         - wraparound_policy;
	 *         - saturation_policy.
	 */
	template<
		unsigned short int Width,
		unsigned short int Fraction,
		bool Signed = true,
		typename BehaviorPolicy = wraparound_policy
	>
	class fixed_point_new {};

	/*
	 [ sign ][ integer ][ fraction ]
	 Например:
	 8 бит всего
	 4 бита дробной части
	 3 бита целой части + знак
	*/

	template<int E, typename T, typename BehaviorPolicy = wraparound_policy>
	class fixed_point {
		static_assert(0 <= E && E <= 16, "The exponent must be greater than 0 and less than or "
			"equal to 16.");

		static_assert((is_signed_v<T> || is_unsigned_v<T>) && numeric_limits<T>::width != 64,
			"Only integer types with a bit width of up to "
			"32 bits inclusive are supported as internal representation (basic_type). 64-bit types"
			"are used by the library as extended intermediate types (wider_type).");

	public:
		using basic_type = T;
		using policy_type = BehaviorPolicy;
		using wider_type = make_wider_type_t<basic_type>;

		static constexpr auto exponent = E;
		static constexpr auto scale = basic_type{ 1 } << E;

		constexpr fixed_point() = default;
		template<typename F, typename = core::enable_if_t<is_floating_point_v<F>>>
		explicit constexpr fixed_point(F number) :
			_rep{ policy_type::template apply<basic_type>(static_cast<wider_type>(number * scale)) } {}

		fixed_point(const fixed_point&) = default;
		fixed_point(fixed_point&&) = default;
		~fixed_point() = default;

		fixed_point& operator=(const fixed_point&) = default;
		fixed_point& operator=(fixed_point&&) = default;

		FORCEINLINE constexpr fixed_point& operator+=(const fixed_point& other) noexcept(
			noexcept(policy_type::template apply<basic_type>(other._rep))
			) {
			const auto sum = static_cast<wider_type>(_rep) + other._rep;
			_rep = policy_type::template apply<basic_type>(sum);
			return *this;
		}
		template<typename U, typename = enable_if_t<is_signed_v<U> || is_unsigned_v<U>
		|| is_floating_point_v<U>>>
			FORCEINLINE constexpr fixed_point& operator+=(U number) noexcept(
				noexcept(policy_type::template apply<basic_type>(number))
				) {
			static_assert(sizeof(U) < sizeof(wider_type), "The type width must be less than the "
				"fixed_point type's width_type.");
			const auto sum = static_cast<wider_type>(_rep) + static_cast<wider_type>(number * scale);
			_rep = policy_type::template apply<basic_type>(sum);
			return *this;
		}

	private:
		basic_type _rep{};
	};

	/*
	* [[nodiscard]] FORCEINLINE constexpr auto rep() const { return _rep; }
		///////////////////////////////////////////////////////////////////////////////////////////
		FORCEINLINE fixed_point& operator-=(const fixed_point& other) {
			_rep -= other._rep;
			return *this;
		}
		template<typename T,
			typename = core::enable_if_t<core::is_signed_v<T> || core::is_unsigned_v<T>>>
		FORCEINLINE fixed_point& operator-=(T number) {
			_rep -= static_cast<basic_type>(number) * scale;
			return *this;
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		FORCEINLINE fixed_point& operator*=(const fixed_point& other) {
			const auto tmp = (static_cast<wider_type>(_rep) * other._rep) >> exponent;
			_rep = get_valid_value(tmp);
			return *this;
		}
		template<typename T,
			typename = core::enable_if_t<core::is_signed_v<T> || core::is_unsigned_v<T>>>
		FORCEINLINE fixed_point& operator*=(T number) {
			_rep *= static_cast<basic_type>(number) * scale;
			return *this;
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		FORCEINLINE fixed_point& operator/=(const fixed_point& other) {
			_rep = (_rep << exponent) / other._rep;
			return *this;
		}
		template<typename T,
			typename = core::enable_if_t<core::is_signed_v<T> || core::is_unsigned_v<T>>>
		FORCEINLINE fixed_point& operator/=(T number) {
			_rep /= static_cast<basic_type>(number) * scale;
			return *this;
		}
		*/

		/*
		///////////////////////////////////////////////////////////////////////////////////////////////
		template<int E>
		[[nodiscard]] FORCEINLINE constexpr fixed_point<E> operator+(fixed_point<E> lhs,
			const fixed_point<E>& rhs) noexcept {
			lhs += rhs;
			return lhs;
		}
		template<int E, typename T,
			typename = core::enable_if_t<core::is_signed_v<T> || core::is_unsigned_v<T>>>
		[[nodiscard]] FORCEINLINE constexpr fixed_point<E> operator+(fixed_point<E> lhs,
			T rhs) noexcept {
			lhs += rhs;
			return lhs;
		}
		template<int E, typename T,
			typename = core::enable_if_t<core::is_signed_v<T> || core::is_unsigned_v<T>>>
		[[nodiscard]] FORCEINLINE constexpr fixed_point<E> operator+(T lhs,
			const fixed_point<E>& rhs) noexcept {
			return rhs + lhs; /// Используя свойство коммутативности, можно изменить порядок аргументов
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		template<int E>
		[[nodiscard]] FORCEINLINE const fixed_point<E> operator-(fixed_point<E> lhs,
			const fixed_point<E>& rhs) {
			lhs -= rhs;
			return lhs;
		}
		template<int E, typename T,
			typename = core::enable_if_t<core::is_signed_v<T> || core::is_unsigned_v<T>>>
		[[nodiscard]] FORCEINLINE const fixed_point<E> operator-(fixed_point<E> lhs, T rhs) {
			lhs -= rhs;
			return lhs;
		}
		template<int E, typename T,
			typename = core::enable_if_t<core::is_signed_v<T> || core::is_unsigned_v<T>>>
		[[nodiscard]] FORCEINLINE const fixed_point<E> operator-(T lhs, const fixed_point<E>& rhs) {
			const auto tmp = fixed_point<E>::make_from_raw(lhs);
			return tmp - rhs;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		template<int E>
		[[nodiscard]] FORCEINLINE constexpr fixed_point<E> operator*(fixed_point<E> lhs,
			const fixed_point<E>& rhs) {
			lhs *= rhs;
			return lhs;
		}
		template<int E, typename T,
			typename = core::enable_if_t<core::is_signed_v<T> || core::is_unsigned_v<T>>>
		[[nodiscard]] FORCEINLINE constexpr fixed_point<E> operator*(fixed_point<E> lhs, T rhs) {
			lhs *= rhs;
			return lhs;
		}
		template<int E, typename T,
			typename = core::enable_if_t<core::is_signed_v<T> || core::is_unsigned_v<T>>>
		[[nodiscard]] FORCEINLINE constexpr fixed_point<E> operator*(T lhs, const fixed_point<E>& rhs) {
			return rhs * lhs; /// Используя свойство коммутативности, можно изменить порядок аргументов
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		template<int E>
		[[nodiscard]] FORCEINLINE const fixed_point<E> operator/(fixed_point<E> lhs,
			const fixed_point<E>& rhs) {
			lhs /= rhs;
			return lhs;
		}
		template<int E, typename T,
			typename = core::enable_if_t<core::is_signed_v<T> || core::is_unsigned_v<T>>>
		[[nodiscard]] FORCEINLINE const fixed_point<E> operator/(fixed_point<E> lhs, T rhs) {
			lhs /= rhs;
			return lhs;
		}
		*/
} /// !namespace core

#endif /// !defined(FIXED_POINT_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)