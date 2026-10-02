/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Функции помощники.
 *
 */

#ifndef UTILITY_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define UTILITY_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <ascii.h>
#include <array.h>
#include <common.h>
#include <numeric_limits.h>
#include <type_traits.h>
#include <wider_type.h>

namespace core {
	/**
	 * @brief Вычисляет длину строки.
	 *
	 * @param [in] str указатель на начало строки
	 * @return         количество символов в строке
	 */
	[[nodiscard]] FORCEINLINE constexpr unsigned long long int length(const char* str) {
		unsigned long long int count{};
		for(; *str != '\0'; ++count)
			++str;
		return count;
	}


	/**
	 * @brief Структура для хранения пары значений.
	 *
	 * Структура может быть полезна для создания шаблонов, которые требуют 
	 * хранения двух связанных значений вместе, например, для создания 
	 * ассоциативных массивов или других структур данных, которые требуют 
	 * хранения пар ключ-значение.
	 *
	 * @tparam T тип первого значения
	 * @tparam U тип второго значения
	 */
	template <typename T, typename U> struct pair { T first; U second; };
	template <typename T, typename U>
	[[nodiscard]] FORCEINLINE constexpr auto make_pair(const T& first, const U& second)
		noexcept {
		return pair<T, U>{first, second};
	}

	/**
	 * @brief Структура-обёртка для массивов.
	 *
	 * Позволяет использовать массивы в контекстах, в которых требуется 
	 * возвращать указатель на массив, время жизни которого определяет 
	 * структура-обёртка.
	 *
	 * @tparam T тип элементов массива
	 * @tparam N размер массива
	 */
	template <typename T, unsigned short int N>
	struct wrapper_for_array {
		template<typename... Args>
		constexpr wrapper_for_array(Args... args) : _value{ args... } {}
		[[nodiscard]] constexpr T(&operator()() noexcept)[N] { return _value; }
		[[nodiscard]] constexpr const T(&operator()() const noexcept)[N] { return _value; }

	private:
		T _value[N]{};
	};

	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		/*
		 * Реализация алгоритма.
		 * Размер массива для знаковых и беззнаковых тип одинаков и равен 
		 * размеру массива для знаковых типов + '\0'.
	     *
	     *            number_to_string()
	     *                    │
	     *                    ├── определяет signed / unsigned
	     *                    ├── определяет знак
	     *                    └── получает magnitude
	     *                              │
	     *                              ▼
	     *                   number_to_string_impl()
	     *                              │
	     *                              ├── получает unsigned
	     *                              ├── извлекает цифры
	     *                              └── формирует строку
		 * 
		 * @tparam T          тип целого числа
		 * @tparam Sign       знак значения, принимает или + или -
		 * @param [in] number число для преобразования
		 * @return            массив символов
		 * 
		 * @note Для значения 0 формируется строка "0".
		 */
		template<typename T, ascii Sign = ascii::plus>
		[[nodiscard]] constexpr auto number_to_string_impl(T number) {
			/*
			 * Буфер имеет размер, достаточный для представления максимального
			 * значения соответствующего знакового типа, плюс один символ под '\0'.
			 * Для беззнакового типа это может быть на один символ больше необходимого,
			 * но такой размер позволяет унифицировать тип результата для знаковых
			 * и беззнаковых аргументов.
			 */
			static_assert(is_unsigned_v<T>, "Only unsigned integer types are supported.");
			static_assert(Sign == ascii::plus || Sign == ascii::minus, "");
			constexpr bool is_negative{ Sign == ascii::minus };

			array<
				char,
				numeric_limits<make_signed_t<T>>::decimal_width + 1ULL
			> numeric_string{};

			/// Ноль является особым случаем: основной алгоритм не выполняется 
			/// ни разу, поскольку условие number != 0 изначально ложно.
			if(number == 0) {
				numeric_string[0] = '0';
				numeric_string[1] = '\0';
				return numeric_string;
			}

			/// Последовательно извлекаем десятичные цифры числа справа налево.
			/// Полученная последовательность записывается в буфер в обратном 
			/// порядке.
			constexpr unsigned short int radix{ 10U };
			constexpr decltype(numeric_limits<make_signed_t<T>>::decimal_width) first{ is_negative ? 1 : 0 };
			auto end = first;

			while(number != 0) {
				numeric_string[end++] = static_cast<char>('0' + (number % radix));
				number /= radix;
			}
			numeric_string[end] = '\0'; /// Завершаем строку нулевым символом.

			/// Вспомогательная функция для обмена двух символов строки.
			auto swap = [&numeric_string](decltype(end) b, decltype(end) e) {
				const auto tmp = numeric_string[b];
				numeric_string[b] = numeric_string[e];
				numeric_string[e] = tmp;
			};

			/// После последовательного извлечения цифр строка находится в 
			/// обратном порядке. Разворачиваем её симметричными обменами 
			/// от краёв к центру.
			--end;
			for(auto beg = first; beg < end; ++beg, --end) {
				swap(beg, end);
			}
			if constexpr(is_negative)
				numeric_string[0] = static_cast<char>(Sign);

			return numeric_string;
		}

	} /// !namespace helper
	/// @endcond

	/**
	* @brief Преобразовывает целое число в строку.
	* 
	* Число преобразуется в последовательность десятичных символов без
	* использования динамической памяти. Результат записывается в
	* символьный массив и завершается нулевым символом '\0'.
	*
	* @tparam T          тип целого числа
	* @param [in] number число для преобразования
	* @return            массив символов
	*
	* @note Для значения 0 формируется строка "0".
	*/
	template<typename T, 
		typename = enable_if_t<is_integral_v<T> && !is_same_v<T, bool> && !is_same_v<T, char>>>
	[[nodiscard]] constexpr auto number_to_string(T number) {
		static_assert(core::is_arithmetic_integer_v<T>, "Only integer types are supported.");
		if constexpr(is_unsigned_v<T>)
			return helper::number_to_string_impl(number);
		else {
			using U = make_unsigned_t<T>;
			if(number >= 0)
				/// Положительное значение можно непосредственно преобразовать 
				/// в соответствующий  беззнаковый тип.
				return helper::number_to_string_impl(static_cast<U>(number));
			else {
				/// Модуль отрицательного числа вычисляется в беззнаковой 
				/// арифметике. Это позволяет корректно обработать минимальное
				/// значение знакового типа, для которого положительного аналога
				/// в том же знаковом типе не существует.
				U magnitude = U{} - static_cast<U>(number);
				return	helper::number_to_string_impl<U, ascii::minus>(magnitude);
			}
		}
	}
	

	 /**
	  * @brief Преобразовывает число с плавающей запятой в строку.
	  *
	  * Число преобразуется в последовательность десятичных символов без
	  * использования динамической памяти. Результат записывается в
	  * символьный массив и завершается нулевым символом '\0'.
	  *
	  * @param [in] number    число для преобразования
	  * @param [in] precision количество знаков после запятой (по умолчанию 6)
	  * @return               массив символов
	  *
	  * @note Для значения 0 формируется строка "0.000000" (с учётом точности).
	  */
	[[nodiscard]] constexpr auto number_to_string(float number, 
		unsigned short int precision = 6U) {
		/*
		 * В стандартах программирования(например, IEEE 754) для чисел с плавающей
		 * запятой определены специальные состояния при выходе за границы диапазона,
		 * обычно от approx 3.4 * 10^38 до approx 3.4 * 10^38.
		 *
		 * Переполнение в большую сторону(Overflow).
		 * Если результат операции превышает максимальное положительное значение,
		 * переменная принимает специальное значение Infinity(бесконечность).
		 *
		 * Переполнение в меньшую сторону(Underflow).
		 * Если число становится ближе к нулю, чем минимальное ненулевое значение,
		 * оно может превратиться в 0 или перейти в денормализованную форму с потерей
		 * точности.
		 *
		 * Неопределенные операции.
		 * Деление бесконечности на бесконечность или извлечение корня из отрицательного
		 * числа дает специальное значение NaN(Not a Number — не число).
		 * В отличие от целочисленного переполнения, программа обычно не падает с ошибкой
		 * и не «зацикливает» значение на минимуме, а продолжает работу со спецзначением
		 * бесконечности.
		 */
		constexpr auto scale = 8388608U;  /**< Масштаб для преобразования. */
		constexpr auto mask = scale - 1U; /**< Маска для извлечения дробной части. */
		
		using Pure_type = pure_type_t<decltype(scale)>;
		using Wider_type = make_wider_type_t<Pure_type>;
		const auto fixed = static_cast<Wider_type>(number * scale); 
		
		auto numeric_string = number_to_string(static_cast<Pure_type>(fixed / scale)); /**< Целая часть числа. */
		const auto period_pos = length(numeric_string.data());

		using Char_type = pure_type_t<decltype(numeric_string)>::value_type;
		numeric_string[period_pos] = static_cast<Char_type>(ascii::period); /**< Разделитель целой и дробной части числа. */

		/*
		 * На данный момент реализовано "грубое" преобразование числа с 
		 * плавающей запятой в строку - "обрезка", без учёта округления.
		 */
		auto frac = static_cast<Pure_type>(fixed & mask); /**< Дробная часть числа. */
		for(auto pos = period_pos + 1U; pos < (period_pos + 1U + precision); ++pos) {
			frac *= 10;
			numeric_string[pos] = static_cast<Char_type>(static_cast<Char_type>(ascii::zero) + 
				(frac >> numeric_limits<float>::mantissa_width));
			frac &= mask;
		}
		numeric_string.back() = '\0';
		return numeric_string;
	}


	/**
	 * @brief     Обменивает значения двух переменных.
	 * @tparam T  тип переменных
	 * @param lhs левая переменная
	 * @param rhs правая переменная
	 */
	template<typename T>
	FORCEINLINE constexpr void swap(T& lhs, T& rhs) noexcept {
		const auto tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}

	/**
	 * @brief Алгоритм последовательно сворачивает элементы в диапазоне
	 *        [first, last) в одно состояние с помощью бинарной операции.
	 *
	 * init задаёт начальное состояние. На каждой итерации текущее состояние
	 * преобразуется операцией op с очередным элементом диапазона: 
	 * state = op(state, *first). Для пустого диапазона результатом является init.
	 * Последовательность вычислений:
	 * init -> op(init, a) -> op(result, b) -> op(result, c) -> ...
	 *
	 * @tparam I    тип итератора, указывающего на элементы диапазона
	 * @tparam U    тип значения, в котором будет вычисляться сумма
	 * @tparam Op   тип бинарной операции, используемой для сворачивания 
	 *              диапазона, не обязательно должен быть арифметической 
	 *              операцией.
	 *
	 * @param first итератор, указывающий на первый элемент диапазона
	 * @param last  итератор, указывающий на элемент за последним элементом 
	 *              диапазона
	 * @param init  значение, которое определяет начальное состояние
	 * @param op    бинарная операция, используемая для сворачивания диапазона,
	 *              задает переход с одного состояния в другое
	 * @return      результат последовательного преобразования состояния, 
	 *              выполняемого операцией op
	 *
	 * @attention Алгоритм не предоставляет гарантий корректности преобразования
	 *            состояния, выполняемого операцией op. Ответственность за 
	 *            корректность и допустимость переходов состояния возлагается
	 *            на op и тип состояния U.
	 *
	 * fold
	 *	 │
	 *	 ├── гарантирует обход диапазона
	 *	 ├── гарантирует последовательность переходов
	 *	 └── вызывает op
	 *			  │
	 *			  ├── определяет семантику вычисления
	 *			  ├── отвечает за переполнение
	 *			  ├── отвечает за точность
	 *			  └── отвечает за допустимость состояния
	 */
	template<typename I, typename U, typename Op>
	[[nodiscard]] constexpr U fold(I first, I last, U init, Op op) noexcept {
		for(; first != last; ++first)
			init = op(init, *first);
		return init;
	}

} /// !namespace core

#endif /// !defined(UTILITY_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)

