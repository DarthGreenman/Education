#include <Arduino.h>
#include "./uno_lib.h"

/** Соберите в симуляторе WOKWI схему, состоящую из платы Arduino UNO, жидкокристаллического индикатора 16 х 2
 * и аналогового джойстика. Джойстик подключите к линиям питания GND и +5V, его аналоговые выводы подключите
 * к аналоговым входам платы Arduino UNO, а выход кнопки джойстика - к любому входу платы Arduino UNO.
 * Подключите жидкокрисллический индикатор к оставшимся свободным выводам платы Arduino UNO.
 * Разработайте программу, в которой:
 *  1. Перемещение по оси Х приводит к перемещению курсора по строке жидкокристаллического индикатора.
 *  2. Перемещение по оси Y - к выбору символа из диапазона от A до Z (обратитесь к таблице кодов ASCII).
 *  3. Нажатие на кнопку джойстика переключает активную строку жидкокристаллического индикатора.
 */

#define _DEBUGGING_ASYNCHRONOUS

using namespace uno;
using control = joystick_controls;
joystick<
	bind<2, control::button>,
	bind<14, control::horizontal_variable_resistor>,
	bind<15, control::vertical_variable_resistor>
> joy{};

task_scheduler<decltype(joy)> task_joy{
	&decltype(joy)::update,
	joy,
	decltype(joy)::default_polling_time
};

using reg = hd44780::registers;
liquid_crystal_display<
	bind<12, reg::rs>, bind<11, reg::e>,
	bind<10, reg::d0>, bind<9, reg::d1>, bind<8, reg::d2>, bind<7, reg::d3>,
	bind<6, reg::d4>, bind<5, reg::d5>, bind<4, reg::d6>, bind<3, reg::d7>
> lcd{};

/// Структура для хранения позиции курсора дисплея.
struct point { int8_t x{}, y{}; } pos{};

/// Массив символов от A до Z.
using s = utl::ascii;
constexpr s symbols[]{
	s::A, s::B, s::C, s::D, s::E, s::F, s::G,
	s::H, s::I, s::J, s::K, s::L, s::M, s::N,
	s::O, s::P, s::Q, s::R, s::S, s::T, s::U,
	s::V, s::W, s::X, s::Y, s::Z
};

/// Массив для хранения последнего выбранного символа. По умолчанию массив инициализируется
/// значением utl::ascii::null
constexpr int8_t number_of_cols{ 16 };
constexpr int8_t number_of_rows{ 2 };
s cells[number_of_cols][number_of_rows]{};

/**
 * @brief  Поиск порядкового номера символа в массиве символов.
 * @return Возвращает порядковый номер символа в массиве символов, если символ найден,
 *         в противном случае возвращает значение размера массива.
 */
auto find = [](utl::ascii symbol) {
	constexpr uint8_t size{ sizeof(symbols) / sizeof(symbols[0]) };
	for(uint8_t i{}; i < size; ++i)
		if(symbols[i] == symbol)
			return i;
	return size;
};

void setup() {
	joy.begin();
	lcd.begin();

#ifdef DEBUGGING_ASYNCHRONOUS
	Serial.begin(115200);
#endif

	/// Перемещает курсор жидкокристаллического дисплея по оси Х влево.
	joy.on_left([]() {
#ifdef DEBUGGING_ASYNCHRONOUS
		Serial.println("Left");
#endif
		if(--pos.x < 0)
			pos.x = 0;
		lcd.set_cursor(pos.x, pos.y);
		; });

	/// Перемещает курсор жидкокристаллического дисплея по оси Х вправо.
	joy.on_right([]() {
#ifdef DEBUGGING_ASYNCHRONOUS
		Serial.println("Right");
#endif
		if(++pos.x >= number_of_cols)
			pos.x = number_of_cols - 1;
		lcd.set_cursor(pos.x, pos.y);
		; });

	/// Выбирает символ из диапазона от A до Z, начиная с A.
	joy.on_up([]() {
#ifdef DEBUGGING_ASYNCHRONOUS
		Serial.println("Up");
#endif
		constexpr uint8_t size{ sizeof(symbols) / sizeof(symbols[0]) };
		const uint8_t value{ find(cells[pos.x][pos.y]) };
		const auto index{ value == size ? 0 : value == size - 1 ? value : value + 1 };

		lcd.print_char(static_cast<uint8_t>(cells[pos.x][pos.y] = symbols[index]));
		lcd.set_cursor(pos.x, pos.y);
		; });

	/// Выбирает символ из диапазона от A до Z, начиная с Z.
	joy.on_down([]() {
#ifdef DEBUGGING_ASYNCHRONOUS
		Serial.println("Down");
#endif
		constexpr uint8_t size{ sizeof(symbols) / sizeof(symbols[0]) };
		const uint8_t value{ find(cells[pos.x][pos.y]) };
		const auto index{ value == size ? size - 1 : value == 0 ? value : value - 1 };

		lcd.print_char(static_cast<uint8_t>(cells[pos.x][pos.y] = symbols[index]));
		lcd.set_cursor(pos.x, pos.y);
		; });

	/// Переключает активную строку жидкокристаллического индикатора.
	joy.on_pressed([]() {
#ifdef DEBUGGING_ASYNCHRONOUS
		Serial.println("Select");
#endif
		lcd.set_cursor(pos.x, pos.y = !pos.y);
		; });
}

void loop() {

	task_joy.exec(true);

#ifdef DEBUGGING_ASYNCHRONOUS
	static uint32_t uptime{};
	if(millis() - uptime >= 1000ul) {
		uptime = millis();
		Serial.println(uptime);
	}
#endif
}