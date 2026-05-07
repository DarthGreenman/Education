
#include "hardware.h"
#include "task.h"
#include "button.h"
#include "signal.h"
#include "types_used.h"
#include "seven_segment_indicator.h"

/**
 * Задача №1. Подсчет числа нажатий на кнопку с помощью одноразрядного семисегментного индикатора
 * Соберите в симуляторе WOKWI схему, состоящую из платы Arduino UNO, семи резисторов номиналом 300 Ом,
 * семисегментного индикатора и кнопки. По умолчанию общий вывод семисегментного индикатора подключен к анодам сегментов,
 * поэтому его необходимо подключить к цепи +5V. Катоды сегментов подключите к любым выводам платы Arduino UNO.
 * Также к любому выводу платы Arduino UNO подключите кнопку.
 * Разработайте программу, которая подсчитывает количество нажатий на кнопку и выводит это значение
 * на семисегментный индикатор в виде шестнадцатеричного кода числа (т.е. цифры от 0 до 9 и буквы: A, b, C, d, E, F).
 * При нажании кнопки в состоянии F должен происходить переход в состояние 0.
 */

using button = uno::button<uno::signal::input_pullup>;
button counter_button{ 2 };
uno::task<button> task_button{ &button::update, &counter_button, 50 };

using S = uno::single_seven_segment_indicator;
uno::seven_segment_indicator seven_segment_display{
	fox::make_pair(6, S::b),
	fox::make_pair(7, S::c),
	fox::make_pair(9, S::e),
	fox::make_pair(10, S::f),
	fox::make_pair(5, S::a),
	fox::make_pair(11, S::g),
	fox::make_pair(12, S::dp),
	fox::make_pair(8, S::d)
};

constexpr S segment_map[]{
	S::a | S::b | S::c | S::d | S::e | S::f,
	S::b | S::c,
	S::a | S::b | S::g | S::e | S::d,
	S::a | S::b | S::g | S::c | S::d,
	S::f | S::g | S::b | S::c,
	S::a | S::f | S::g | S::c | S::d,
	S::a | S::f | S::g | S::c | S::d | S::e,
	S::a | S::b | S::c,
	S::a | S::b | S::c | S::d | S::e | S::f | S::g,
	S::a | S::b | S::c | S::d | S::f | S::g,
	S::a | S::b | S::c | S::e | S::f | S::g,
	S::f | S::g | S::c | S::d | S::e,
	S::a | S::f | S::e | S::d,
	S::b | S::c | S::d | S::e | S::g,
	S::a | S::f | S::g | S::e | S::d,
	S::a | S::f | S::g | S::e
};

void setup() {
	auto action = []() {
		static uint8_t count{};
		seven_segment_display.show(segment_map[count++]);
		if(count == sizeof(segment_map) / sizeof(segment_map[0]))
			count = 0;
	};
	counter_button.pressed(action);
}

void loop() {
	task_button.exec(true);
}