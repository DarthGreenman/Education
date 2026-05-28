/// types_used.h

#ifndef TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT
#define TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "fox.h"
#include <stdint.h>

namespace uno {
	/**********************************************************************************************
	 * @brief Ошибки времени исполнения
	 * already_in_use_pin  Значение указывает на то, что pin-код уже используется,
	 *                     был назначен другому компоненту.
	 * invalid_pin         Значение указывает на то, что pin-код, указанный для
	 *                     какого-либо компонента недействителен или не существует
	 *                     на плате Arduino Uno.
	 */
	enum class
		error :	uint8_t {
		already_in_use_pin = 0xfe,
		invalid_pin = 0xff
	};

	/**********************************************************************************************
	 * @brief Тип подключения к общему контакту
	 * anode   Common Anode, для включения компонента необходимо подать низкий уровень
	 *         сигнала (LOW) на его катoд
	 * cathode Common Cathode, для включения сегмента необходимо подать ВЫСОКИЙ уровень
	 *         сигнала (HIGH) на его анod
	 */
	enum class
		common_pin : uint8_t {
		anode, cathode
	};

	/**********************************************************************************************
	 * @brief Цвета для RGB-светодиода
	 * 31–24 - альфа-канал
	 * 23-16 - red
	 *  15-8 - green
	 *   7-0 - blue
	 */
	enum class
		color : uint32_t {
		red = 0x00ff0000, green = 0x0000ff00, blue = 0x000000ff
	};

	/**********************************************************************************************
	 * @brief Пины для подключения компонентов
	 * r1, r2, r3, r4 - строки мембранной клавиатуры
	 * c1, c2, c3, c4 - столбцы мембранной клавиатуры
	 */
	enum class
		membrane_keypad_channels : uint32_t {
		r1 = 0x1000'0000, r2 = 0x0100'0000, r3 = 0x0010'0000, r4 = 0x0001'0000,
		c1 = 0x0000'1000, c2 = 0x0000'0100,	c3 = 0x0000'0010, c4 = 0x0000'0001
	};

	/**********************************************************************************************
	 * @brief Сегменты для одноразрядного индикатора
	 * a, b, c, d, e, f, g, dp - сегменты семисегментного индикатора
	 */
	enum class
		single_digit_indicator_segments : uint32_t {
		a = 0x1000'0000, b = 0x0100'0000, c = 0x0010'0000, d = 0x0001'0000,
		e = 0x0000'1000, f = 0x0000'0100, g = 0x0000'0010, dp = 0x0000'0001
	};

	/**********************************************************************************************
	 * @brief Значения разрядов для декомпозиции числа для отображения на четырёхразрядном индикаторе
	 * units, tens, hundreds, thousands - единицы, десятки, сотни и тысячи соответственно
	 */
	enum class
		digit_of_the_number : uint16_t {
		units = 0x0001, tens = 0x000a, hundreds = 0x0064, thousands = 0x3e8
	};

	/**********************************************************************************************
	 * @brief Операторы для перечислений, которые позволяют использовать их значения в битовых операциях
	 * (|, ^, &, ~) и сравнениях (==, !=) без необходимости явного приведения типов.
	 * Эти операторы перегружены для всех перечислений, что обеспечивает удобство использования
	 * и улучшает читаемость кода при работе с битовыми масками и флагами.
	 */
	template<typename E> constexpr inline E operator | (E lhs, E rhs) noexcept {
		return static_cast<E>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}
	template<typename E> constexpr inline E operator | (E lhs, uint32_t rhs) noexcept {
		return static_cast<E>(static_cast<uint32_t>(lhs) | rhs);
	}
	template<typename E> constexpr inline E operator | (uint32_t lhs, E rhs) noexcept {
		return static_cast<E>(lhs | static_cast<uint32_t>(rhs));
	}

	template<typename E> constexpr inline E operator ^ (E lhs, E rhs) noexcept {
		return static_cast<E>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
	}
	template<typename E> constexpr inline E operator ^ (E lhs, uint32_t rhs) noexcept {
		return static_cast<E>(static_cast<uint32_t>(lhs) ^ rhs);
	}
	template<typename E> constexpr inline E operator ^ (uint32_t lhs, E rhs) noexcept {
		return static_cast<E>(lhs ^ static_cast<uint32_t>(rhs));
	}

	template<typename E> constexpr inline E operator & (E lhs, E rhs) noexcept {
		return static_cast<E>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}
	template<typename E> constexpr inline E operator & (E lhs, uint32_t rhs) noexcept {
		return static_cast<E>(static_cast<uint32_t>(lhs) & rhs);
	}
	template<typename E> constexpr inline E operator & (uint32_t lhs, E rhs) noexcept {
		return static_cast<E>(lhs & static_cast<uint32_t>(rhs));
	}

	/**********************************************************************************************
	 * @brief Структура для привязки пинов к компонентам
	 * 
	 * @tparam P 
	 * @tparam B 
	 */
	template <uint8_t P, auto B> struct binder {
		constexpr static auto pin = P;
		constexpr static auto bind = B;
	};

	/**********************************************************************************************
	 * @brief Структура для получения привязки по индексу
	 * 
	 * @tparam N 
	 * @tparam B 
	 */
	template <uint8_t N, typename B> struct get_bind {};
	template <typename B> struct get_bind<0, B> { constexpr static auto value = B::pin; };
	template <typename B> struct get_bind<1, B> { constexpr static auto value = B::bind; };

	/**********************************************************************************************
	 * @brief Структура-обёртка для массивов, которая позволяет использовать их в контекстах,
	 * где требуется возвращать массивы из функций или передавать их в шаблонные функции.
	 * 
	 * @tparam T Тип элементов массива
	 * @tparam N Размер массива
	 */
	template <typename T, uint8_t N>
	struct wrapper_for_array{ T value[N]{}; };

} /// !namespace uno

#endif /// !TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT
