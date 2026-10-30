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

namespace drv {
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

using namespace uno;
auto turn_on_time = 1_sec; //**< Время в течении которого led активный. */

auto print = [](uint32_t pos_value, int value) {
	drv::display.set_cursor(pos_value, 0);
	drv::display.write(value, 2);
};

#define _DEBUG_LED

void setup() {
#ifdef DEBUG_LED
	Serial.begin(115200);
#endif

	using namespace drv;

	sensor.begin();
	led.begin();

	display.begin();
	static constexpr const char* promt{ "FLICKER TIME:" };
	display.write(promt);

	static constexpr auto pos = core::length(promt);
	static constexpr core::pair<int, int> range{ 2, 10 };
	static auto index = range.first;

	print(pos, index);
	stick.begin();

	/// Обработчики событий джойстика
	/**< Увеличить время активности. */
	stick.on_up([]() { if(index < range.second - 1) { print(pos, ++index);	} });
	/**< Уменьшить время активности. */
	stick.on_down([]() { if(index > range.first) { print(pos, --index); } });
	/**< Выбрать текущее время активности. */
	stick.on_pressed([]() {turn_on_time = index * 1_sec; });
}

void loop() {
	static decltype(millis()) uptime_led{}; //**< Момент времени последнего события led. */
	const auto uptime = millis();

	using namespace drv;

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
