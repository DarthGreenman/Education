
#include "ascii.h"
#include "fox.h"
#include "keypad.h"
#include "led_rgb.h"
#include "task.h"

/**
 * @brief Соберите в симуляторе Wokwi схему, состоящую из платы Arduino UNO, трёх резисторов номиналом 300 Ом,
 * одного трёхцветного светодиода (RGB LED) и одной цифровой клавиатуры. По умолчанию общий вывод трёхцветного
 * светодиода объединяет аноды светодиодов, то есть к нему нужно подвести +5V. Соберите схему так, чтобы красный,
 * зелёный и синий сегменты светодиода управлялись разными выводами платы Arduino UNO.
 * Обратите внимание, что при таком включении светодиод активен, когда на выходе платы Arduino UNO логический ноль.
 * Цифровую клавиатуру подключите к оставшимся свободным выводам платы Arduino UNO.
 * Разработайте программу, в которой нажатие на клавишу от 0 до 7 приводит к включению соответствующего режима
 * светодиода, где:
 *  0 - светодиод выключен
 *  7 - включены все три его сегмента
 *  А - остановить моргание
 *  B - моргать с периодом 1 секунды
 *  С - моргать с периодом 0,5 секунды
 *  D - моргать с периодом 0,25 секунды
 */

#define _DEBUGGING_COLOR

/**
 * @brief
 *
 */
using keypad = uno::keypad<uno::membrane_keypad>;
keypad small{fox::make_pair(5, uno::membrane_keypad::C1), fox::make_pair(4, uno::membrane_keypad::C2),
	fox::make_pair(3, uno::membrane_keypad::C3), fox::make_pair(2, uno::membrane_keypad::C4), fox::make_pair(12, uno::membrane_keypad::R1),
	fox::make_pair(8, uno::membrane_keypad::R2), fox::make_pair(7, uno::membrane_keypad::R3), fox::make_pair(6, uno::membrane_keypad::R4)};
uno::task<keypad> task{&keypad::update, &small, 125};
/**
 * @brief
 *
 */
using led_rgb = uno::light_emitting_diode_rgb<uno::signal_analog>;
using pins = typename uno::light_emitting_diode_rgb<uno::signal_analog>::pins;

led_rgb led{pins{/*red*/ 11, /*green*/ 10, /*blue*/ 9}, led_rgb::common_pin::anode};
static uint32_t color{uno::color::red | uno::color::green | uno::color::blue};

/**
 * @brief
 *
 */
enum class task_duration : uint16_t
{
	A,
	B = 1000,
	C = 500,
	D = 250
};
static task_duration timeout{};

void setup()
{
	Serial.begin(115200);

	using namespace common;
	small.assign_names(ascii::one, ascii::two, ascii::three, ascii::A, ascii::four, ascii::five, ascii::six, ascii::B, ascii::seven,
		ascii::eight, ascii::nine, ascii::C, ascii::asterisk, ascii::zero, ascii::number, ascii::D);

	auto action = []()
	{
		const auto key = small.get_key();
		using namespace uno::color;
		switch (key)
		{
		case ascii::one:
#ifdef DEBUGGING_COLOR
			Serial.println("Red");
#endif
			led.color_change(color = red);
			break;
		case ascii::two:
#ifdef DEBUGGING_COLOR
			Serial.println("Green");
#endif
			led.color_change(color = green);
			break;
		case ascii::three:
#ifdef DEBUGGING_COLOR
			Serial.println("Blue");
#endif
			led.color_change(color = blue);
			break;
		case ascii::four:
#ifdef DEBUGGING_COLOR
			Serial.println("Yellow");
#endif
			led.color_change(color = red | green);
			break;
		case ascii::five:
#ifdef DEBUGGING_COLOR
			Serial.println("Purple");
#endif
			led.color_change(color = red | blue);
			break;
		case ascii::six:
#ifdef DEBUGGING_COLOR
			Serial.println("Azure");
#endif
			led.color_change(color = green | blue);
			break;
		case ascii::seven:
#ifdef DEBUGGING_COLOR
			Serial.println("White");
#endif
			led.color_change(color = red | green | blue);
			break;
		case ascii::zero:
#ifdef DEBUGGING_COLOR
			Serial.println("Dark");
#endif
			led.color_change(color = ~(red | green | blue));
			break;
		case ascii::A:
			timeout = task_duration::A;
			break;
		case ascii::B:
			timeout = task_duration::B;
			break;
		case ascii::C:
			timeout = task_duration::C;
			break;
		case ascii::D:
			timeout = task_duration::D;
			break;
		default:
			break;
		};
	};
	small.key_down(action);
}

void loop()
{
	task.exec(true);

	static uint32_t uptime{};
	if (static_cast<bool>(timeout) && millis() - uptime >= static_cast<uint16_t>(timeout))
	{
		uptime = millis();
		led.blink(color);
	}
}