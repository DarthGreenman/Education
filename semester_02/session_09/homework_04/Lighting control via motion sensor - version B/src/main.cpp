#include <Arduino.h>
#include <math.h>
#include <uno.h>

/*
 * Соберите в симуляторе Wokwi схему состоящую из:
 * 1. Платы Arduino UNO.
 * 2. Жидкокристаллического индикатора 16 х 2.
 * 3. Аналогового джойстика.
 * 4. Резистора номиналом 300 Ом.
 * 5. Светодиода.
 * 6. Датчика движения.
 *
 * Джойстик подключите к линиям питания GND и +5V, его аналоговые выводы
 * подключите к аналоговым входам платы Arduino UNO, а выход кнопки джойстика
 * к любому входу платы Arduino UNO.
 *
 * Подключите жидкокристаллический индикатор, светодиод и датчик движения к
 * оставшимся свободным выводам платы Arduino UNO.
 *
 * Разработайте программу, в которой светодиод включается при наличии сигнала
 * от датчика движения. Светодиод должен выключиться через время, которое
 * задаётся с помощью джойстика и жидкокристаллического индикатора, алгоритм
 * взаимодействия джойстика и индикатора нужно разработать самостоятельно.
 * Время работы светодиода должно задаваться в диапазоне от 2 до 10 секунд
 * с шагом 1 секунда.
 */

namespace {
	/// Объекты
	using namespace uno;

	motion_sensor<2> sensor{};
	light_emitting_diode<digital_signal, bind<13>> led{};

	using reg = hd44780::registers;
	liquid_crystal_display<
		bind<12, reg::rs>, bind<11, reg::e>,
		bind<10, reg::d0>, bind<9, reg::d1>, bind<8, reg::d2>, bind<7, reg::d3>,
		bind<6, reg::d4>, bind<5, reg::d5>, bind<4, reg::d6>, bind<3, reg::d7>
	> display{};

	using control = joystick_controls;
	using joystick = analog_joystick<
		bind<A2, control::button>,
		bind<A1, control::horizontal_variable_resistor>,
		bind<A0, control::vertical_variable_resistor>
	>;
	joystick stick{};

	/// Задачи
	using namespace control;
	task_scheduler<
		decltype(sensor),
		decltype(sensor)::signal_type::value_type>::task_scheduler_impl<&decltype(sensor)::update
		> task_sensor{ sensor };
	task_scheduler<joystick, void>::task_scheduler_impl<&joystick::update> task_stick{ stick };

}

constexpr core::array<core::ascii, 8> symbols{
	core::ascii::two, core::ascii::three, core::ascii::four, core::ascii::five, core::ascii::six,
	core::ascii::seven, core::ascii::eight, core::ascii::nine
};
core::pure_type_t<decltype(symbols)>::size_type index{};
auto turn_on_time = (symbols.front() ^ core::ascii::zero) * 1_sec; //**< Время в течении которого led активный. */

auto print = [](uint32_t pos_symbol, core::ascii symbol){
	display.set_cursor(pos_symbol, 0);
	display.write(static_cast<char>(symbol));
};

#define _DEBUG_LED

void setup() {
#ifdef DEBUG_LED
	Serial.begin(115200);
#endif

	sensor.begin();
	led.begin();

	display.begin();
	static constexpr const char* promt{ "FLICKER TIME:" };
	display.write(promt);

	static constexpr auto pos_symbol = core::length(promt);
	print(pos_symbol, symbols.front());
	stick.begin();

	/// Обработчики событий джойстика
	 /**< Увеличить время активности. */
	stick.on_up([]() { if(index < symbols.size() - 1) { print(pos_symbol, symbols[++index]); } });
	/**< Уменьшить время активности. */
	stick.on_down([]() { if(index > 0) { print(pos_symbol, symbols[--index]); }	});
	/**< Выбрать текущее время активности. */
	stick.on_pressed([]() { turn_on_time = (symbols[index] ^ core::ascii::zero) * 1_sec; });  
}

void loop() {
	static decltype(millis()) uptime_led{}; //**< Момент времени последнего события led. */
	const auto uptime = millis();

	task_stick.exec(uptime, decltype(stick)::default_polling_time);
	
	if(task_sensor.exec(uptime, decltype(sensor)::default_polling_time) && !led.is_on()) {
		led.on();
		uptime_led = uptime;
#ifdef DEBUG_LED
		Serial.println("led.on()");
		Serial.println(uptime_led);
#endif
	}

	if((uptime - uptime_led >= turn_on_time) && led.is_on()) {
		led.off();
		uptime_led = uptime;
#ifdef DEBUG_LED
		Serial.println("led.off()");
		Serial.println(uptime_led);
#endif
	}
}
