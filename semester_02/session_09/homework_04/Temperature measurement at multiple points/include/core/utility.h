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
#include <common.h>
#include <numeric_limits.h>
#include <type_traits.h>

namespace core {
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
		 * @tparam T     тип целого числа
		 * @tparam Sign  знак значения, принимает или + или -
		 * @param number число для преобразования
		 * @return       труктуру-обёртку в котором хранится массив символов
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

			wrapper_for_array<
				char,
				numeric_limits<make_signed_t<T>>::decimal_width + 1ULL
			> container{};

			/// Ноль является особым случаем: основной алгоритм не выполняется 
			/// ни разу, поскольку условие number != 0 изначально ложно.
			if(number == 0) {
				container()[0] = '0';
				container()[1] = '\0';
				return container;
			}

			/// Последовательно извлекаем десятичные цифры числа справа налево.
			/// Полученная последовательность записывается в буфер в обратном 
			/// порядке.
			constexpr unsigned short int radix{ 10U };
			constexpr decltype(numeric_limits<make_signed_t<T>>::decimal_width) first{ is_negative ? 1 : 0 };
			auto end = first;

			while(number != 0) {
				container()[end++] = static_cast<char>('0' + (number % radix));
				number /= radix;
			}
			container()[end] = '\0'; /// Завершаем строку нулевым символом.

			/// Вспомогательная функция для обмена двух символов строки.
			auto swap = [&container](decltype(end) b, decltype(end) e) {
				const auto tmp = container()[b];
				container()[b] = container()[e];
				container()[e] = tmp;
			};

			/// После последовательного извлечения цифр строка находится в 
			/// обратном порядке. Разворачиваем её симметричными обменами 
			/// от краёв к центру.
			--end;
			for(auto beg = first; beg < end; ++beg, --end) {
				swap(beg, end);
			}
			if constexpr(is_negative)
				container()[0] = static_cast<char>(Sign);

			return container;
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
	* @tparam T     тип целого числа
	* @param number число для преобразования
	* @return       труктуру-обёртку в котором хранится массив символов
	*
	* @note Для значения 0 формируется строка "0".
	*/
	template<typename T>
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
	 * @brief Вычисляет длину строки.
	 * @param  str указатель на начало строки
	 * @return количество символов в строке
	 */
	[[nodiscard]] FORCEINLINE constexpr unsigned long long int length(const char* str) {
		unsigned long long int count{};
		for(; *str != '\0'; ++count)
			++str;
		return count;
	}

	/**
	 * @brief Обменивает значения двух переменных.
	 *
	 * @tparam T  тип переменных
	 * @param lhs левая переменная
	 * @param rhs правая переменная
	 */
	template<typename T>
	void swap(T& lhs, T& rhs) noexcept {
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

