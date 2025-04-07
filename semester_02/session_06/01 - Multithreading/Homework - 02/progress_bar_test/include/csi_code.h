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
 ����������� ������������������ ANSI(����.�ANSI escape sequences�) - �����������
 ���������� ��������, ������� ������������ ��� ���������� ��������, ������ � �������
 ������� �������� ���� ���������� � ������� ������. ������������������ ANSI ����
 ����������� ��� ������ �������� ��� ���������� �����������. ��������� �����������
 ������������������ ANSI � �� ��������:

 \x1b[A			����������� ������� �����
 \x1b[B			����������� ������� ����
 \x1b[C			����������� ������� ������
 \x1b[D			����������� ������� �����
 \x1b[xG		� ������� x ������� ������
 \x1b[yd		� ������ y
 \x1b[y;xH		� ������� x,y
 \x1b7			��������� �������
 \x1b8			������� � ����������� �������

 \x1b[2J		�������� �����
 \x1b[0K		�������� �� ����� ������
 \x1b[n@		�������� n ��������
 \x1b[nP		������� n ��������
 \x1b[nL		�������� n ����� ����� �������
 \x1b[nM		������� n �����, ������� � �������
 \x1b]0;t\g		���������� ��������� t ��� ����������� ����

 \x1b[?12h		��������� ������� �������
 \x1b[?12l		���������� ������� �������
 \x1b[?25h		��������� �������
 \x1b[?25l		���������� �������

 \x1b[0m		���������� ���� �� ���������
 \x1b[4m		�������������
 \x1b[24m		��� �������������
 \x1b[1m		������� (���������)
 \x1b[22m		��� �������
 \x1b[7m		�������� ������
 \x1b[27m		������ ��������
 \x1b[fm		���������� ���� ��������, f: 30, ..37
 \x1b[bm		���������� ���� ����, b: 40, ...47

 0 �������� ��� ��������
 1 Bright (���������� �������)
 2 Dim (����������)
 4 Underscore (������ �������������)
 5 Blink (�������)
 7 Reverse (��������)
 8 Hidden (������ �����)

 Foreground Colours (���� ����, ���� ������ ��������� �����):
 30 Black (������)
 31 Red (�������)
 32 Green (�������)
 33 Yellow (������)
 34 Blue (�����)
 35 Magenta (���������)
 36 Cyan (�������)
 37 White (�����)
 37 Light gray
 90 Dark gray
 91 Light red
 92 Light green
 93 Light yellow
 94 Light blue
 95 Light magenta
 96 Light cyan
 97 White

 Background Colours (���� ������, ��� ���� ����):
 40 Black (������)
 41 Red (�������)
 42 Green (�������)
 43 Yellow (������)
 44 Blue (�����)
 45 Magenta (���������)
 46 Cyan (�������)
 47 White (�����)
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
