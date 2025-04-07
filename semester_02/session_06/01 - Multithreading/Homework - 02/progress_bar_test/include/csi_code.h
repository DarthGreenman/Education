// csi_code.h

#ifndef CSI_CODE_
#define CSI_CODE_

#include "my_types.h"

#include <cassert>
#include <iosfwd>
#include <string>
#include <utility>
#include <windows.h>

//#include <consoleapi2.h>
//#include <processenv.h>

/*
 Управляющие последовательности ANSI(англ.«ANSI escape sequences») - специальные
 комбинации символов, которые используются для управления форматом, цветом и другими
 опциями внешнего вида выводимого в консоль текста. Последовательности ANSI были
 разработаны как единый стандарт для управления терминалами. Некоторые управляющие
 последовательности ANSI и их действия:

 \x1b[A			Перемещение курсора вверх
 \x1b[B			Перемещение курсора вниз
 \x1b[C			Перемещение курсора вправо
 \x1b[D			Перемещение курсора влево
 \x1b[xG		В позицию x текущей строки
 \x1b[yd		В строку y
 \x1b[y;xH		В позицию x,y
 \x1b7			Запомнить позицию
 \x1b8			Перейти в запомненную позицию

 \x1b[2J		Очистить экран
 \x1b[0K		Очистить до конца строки
 \x1b[n@		Вставить n пробелов
 \x1b[nP		Удалить n символов
 \x1b[nL		Вставить n строк перед текущей
 \x1b[nM		Удалить n строк, начиная с текущей
 \x1b]0;t\g		Установить заголовок t для консольного окна

 \x1b[?12h		Включение мигания курсора
 \x1b[?12l		Выключение мигания курсора
 \x1b[?25h		Включение курсора
 \x1b[?25l		Выключение курсора

 \x1b[0m		Установить цвет по умолчанию
 \x1b[4m		Подчеркивание
 \x1b[24m		Без подчеркивания
 \x1b[1m		Яркость (выделение)
 \x1b[22m		Без яркости
 \x1b[7m		Инверсия цветов
 \x1b[27m		Убрать инверсию
 \x1b[fm		Установить цвет символов, f: 30, ..37
 \x1b[bm		Установить цвет фона, b: 40, ...47

 0 Сбросить все атрибуты
 1 Bright (повышенная яркость)
 2 Dim (затемнение)
 4 Underscore (нижнее подчеркивание)
 5 Blink (мигание)
 7 Reverse (инверсия)
 8 Hidden (скрыть текст)

 Foreground Colours (цвет пера, этим цветом выводится текст):
 30 Black (черный)
 31 Red (красный)
 32 Green (зеленый)
 33 Yellow (желтый)
 34 Blue (синий)
 35 Magenta (пурпурный)
 36 Cyan (голубой)
 37 White (белый)
 37 Light gray
 90 Dark gray
 91 Light red
 92 Light green
 93 Light yellow
 94 Light blue
 95 Light magenta
 96 Light cyan
 97 White

 Background Colours (цвет бумаги, или цвет фона):
 40 Black (черный)
 41 Red (красный)
 42 Green (зеленый)
 43 Yellow (желтый)
 44 Blue (синий)
 45 Magenta (пурпурный)
 46 Cyan (голубой)
 47 White (белый)
*/

namespace csi
{
	
	auto get_console_size()
	{
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		return std::make_pair(info.srWindow.Right - info.srWindow.Left + 1, 
			info.srWindow.Bottom - info.srWindow.Top + 1);
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class foreground_color : int
	{
		black = 30,
		red,
		green,
		yellow,
		blue,
		magenta,
		cyan,
		light_gray,
		dark_gray = 90,
		light_red,
		light_green,
		light_yellow,
		light_blue,
		light_magenta,
		light_cyan,
		white
	}; // enum class foreground_color

	std::ostream& operator<<(std::ostream& os, foreground_color color)
	{
		const std::string csi_code{ "\x1b[" + std::to_string(static_cast<int>(color)) + "m" };
		return (os << csi_code);
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class cursor_event_control : int
	{
		disable = 0,
		enable,
		unblink,
		blink
	}; // enum class cursor_event_control

	std::ostream& operator<<(std::ostream& os, cursor_event_control event)
	{
		const std::string csi_code{
			event == cursor_event_control::disable ? "\x1b[?25l" : event == cursor_event_control::enable ? "\x1b[?25h" :
			event == cursor_event_control::unblink ? "\x1b[?12l" : "\x1b[?12h"
		};
		return (os << csi_code);
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class cursor_position_step : int
	{
		up = my::ascii::A,    // 0b01000001
		down = my::ascii::B,  // 0b01000010
		right = my::ascii::C, // 0b01000011
		left = my::ascii::D   // 0b01000100
	}; // enum class cursor_position_step

	std::ostream& operator<<(std::ostream& os, cursor_position_step position)
	{
		const std::string csi_code{ "\x1b[" + std::string{static_cast<char>(position)} };
		return (os << csi_code);
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct cursor_position_move
	{
		static auto col(unsigned short x)
		{
			const auto&& [cols, rows] = get_console_size();
			assert((x < cols) && "Value outside the console's coordinate(x) limit.");
			return std::string{ "\x1b[" + std::to_string(x) + "G" };
		}
		static auto row(unsigned short y) 
		{
			const auto&& [cols, rows] = get_console_size();
			assert((y < rows) && "Value outside the console's coordinate(y) limit.");
			return std::string{ "\x1b[" + std::to_string(y) + "d" };
		}
		static auto pos(unsigned short x, unsigned short y)
		{
			const auto&& [cols, rows] = get_console_size();
			assert((x < cols) && "Value outside the console's coordinate(x) limit.");
			assert((y < rows) && "Value outside the console's coordinate(y) limit.");
			return std::string{"\x1b[" + std::to_string(y) + ";" + std::to_string(x) + "H" };
		}
	}; // struct cursor_position_move

} // namespace csi

#endif // !CSI_CODE_
