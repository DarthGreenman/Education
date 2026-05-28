/**Секундомер с точностью до целого значения секунды.
 * Интерфейс:
 * 1. Кнопка Старт/Стоп, при первом нажатии разрешается работа секундомера, при втором - работа приостанавливается.
 *    При этом следующее нажатие на кнопку Старт/Стоп должно разрешить работу секундомера без сброса текущего значения времени.
 * 2. Кнопка Сброс, активна только конда секундомер остановлен. При ее нажатии секундомер сбрасывется в ноль.
 */

#include "four_digit_indicator.h"
#include "task.h"
#include "button.h"

#define _DEBUGGING_ASYNCHRONOUS

using namespace uno;
using s = uno::single_digit_indicator_segments;
using d = uno::digit_of_the_number;

// Объекты
uno::four_digit_indicator<
	common_pin::anode,
	binder<15, d::units>,
	binder<16, d::tens>,
	binder<17, d::hundreds>,
	binder<14, d::thousands>,
	binder<7, s::a>,
	binder<9, s::b>,
	binder<11, s::c>,
	binder<12, s::d>,
	binder<13, s::e>,
	binder<8, s::f>,
	binder<10, s::g>
> display{};
uno::button<uno::signal::input_pullup, 3> start{};
uno::button<uno::signal::input_pullup, 2> reset{};

// Задачи
uno::task<decltype(display), uint32_t> display_task{ &decltype(display)::show, &display };
uno::task<decltype(start)> start_task{ &decltype(start)::update, &start, 75 };
uno::task<decltype(reset)> reset_task{ &decltype(reset)::update, &reset, 75 };

auto convert_to_format = [](uint32_t value) {
	const auto min = value / 60;
	const auto sec = value % 60;
	return static_cast<uint32_t>(min * 100 + sec);
};

bool is_run{ false };
uint32_t sec{};

void setup() {
#ifdef DEBUGGING_ASYNCHRONOUS
	Serial.begin(115200);
#endif
	start.pressed([]() { is_run = !is_run; });
	reset.pressed([]() { if(!is_run) sec = 0ul; });
}

void loop() {
	static uint32_t uptime{};
	if(millis() - uptime >= 1000ul) {
		uptime = millis();
		if(is_run)
			++sec;
#ifdef DEBUGGING_ASYNCHRONOUS
		Serial.println(uptime);
#endif
	}
	start_task.exec();
	reset_task.exec();
	display_task.exec(convert_to_format(sec));
}