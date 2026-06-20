/// types_used.h

#ifndef TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT
#define TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <stdint.h>

namespace uno {

	/**********************************************************************************************
	 * @brief Цвета для RGB-светодиода
	 * 31–24 - альфа-канал
	 * 23-16 - red
	 *  15-8 - green
	 *   7-0 - blue
	 */
	enum class
		color : uint32_t {
		red = 0x00ff'0000, green = 0x0000'ff00, blue = 0x0000'00ff,
		white = 0x00ff'ffff, black = 0x0000'0000
	};

	/**********************************************************************************************
	 * @brief Структура элементов управления джойстика
	 */
	enum class
		joystick_controls : uint8_t {
		button, horizontal_variable_resistor, vertical_variable_resistor
	};

	/**********************************************************************************************
	 * @brief Пины для подключения компонентов
	 * r1, r2, r3, r4 - строки мембранной клавиатуры
	 * c1, c2, c3, c4 - столбцы мембранной клавиатуры
	 */
	enum class
		membrane_keypad_channels : uint8_t {
		r1 = 0b1000'0000, r2 = 0b0100'0000, r3 = 0b0010'0000, r4 = 0b0001'0000,
		c1 = 0b0000'1000, c2 = 0b0000'0100, c3 = 0b0000'0010, c4 = 0b0000'0001
	};

	/**********************************************************************************************
	 * @brief Сегменты для одноразрядного индикатора
	 * a, b, c, d, e, f, g, dp - сегменты семисегментного индикатора
	 */
	enum class
		single_digit_indicator_segments : uint8_t {
		a = 0b1000'0000, b = 0b0100'0000, c = 0b0010'0000, d = 0b0001'0000,
		e = 0b0000'1000, f = 0b0000'0100, g = 0b0000'0010, dp = 0b0000'0001
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
	 * @brief Структура элементов управления контроллера
	 */
	namespace hd44780 {
		enum class registers : uint8_t {
			rs = 0b1111'0000, e = 0b0000'1111,
			d7 = 0b1000'0000, d6 = 0b0100'0000, d5 = 0b0010'0000, d4 = 0b0001'0000,
			d3 = 0b0000'1000, d2 = 0b0000'0100, d1 = 0b0000'0010, d0 = 0b0000'0001
		};
	}

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
	template <uint8_t P, auto B> struct bind {
		constexpr static auto pin = P;
		constexpr static auto elem = B;
	};

	/**********************************************************************************************
	 * @brief Структура для получения привязки по индексу
	 *
	 * @tparam N
	 * @tparam B
	 */
	template <uint8_t N, typename B> struct get_bind {};
	template <typename B> struct get_bind<0, B> { constexpr static auto value = B::pin; };
	template <typename B> struct get_bind<1, B> { constexpr static auto value = B::elem; };

	/**********************************************************************************************
	 * @brief
	 */
	struct point_in_axes {
		int16_t x{};
		int16_t y{};
	};

} /// !namespace uno

#endif /// !TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT

