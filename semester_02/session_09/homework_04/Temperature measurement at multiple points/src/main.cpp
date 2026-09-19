#include <Arduino.h>
#include "uno.h"

/**************************************************************************************************
 * Разработайте программу, которая считывает показания датчиков температуры с периодичностью 100 мс,
 * выполняет их программную фильтрацию с помощью усреднения и выводит на жидкокристаллический
 * индикатор следующую информацию:
 * 1. Максимальное значение температуры из трёх датчиков.
 * 2. Минимальное значение температуры из трёх датчиков.
 * 3. Среднее значение температуры по трём датчикам.
 ***********************************************************************************************************/
namespace {
	using namespace uno;

	using reg = hd44780::registers;
	liquid_crystal_display<
		bind<12, reg::rs>, bind<11, reg::e>,
		bind<10, reg::d0>,
		bind<9, reg::d1>,
		bind<8, reg::d2>,
		bind<7, reg::d3>,
		bind<6, reg::d4>,
		bind<5, reg::d5>,
		bind<4, reg::d6>,
		bind<3, reg::d7>
	> lcd{};

	using temperature_calculation_algorithm = math::temperature_calculation<3950>;
	using computing_policy = math::ema<analog_signal::value_type, math::real_number<55, -2>>;

	thermistor<A0, temperature_calculation_algorithm, computing_policy> ntc1{};
	thermistor<A1, temperature_calculation_algorithm, computing_policy> ntc2{};
	thermistor<A2, temperature_calculation_algorithm, computing_policy> ntc3{};

	using namespace control;
	task_scheduler<
		decltype(ntc1),
		100,
		bool>::task_scheduler_impl<&decltype(ntc1)::update>
		task_ntc1{ ntc1 };

	task_scheduler<
		decltype(ntc2),
		100,
		bool>::task_scheduler_impl<&decltype(ntc2)::update>
		task_ntc2{ ntc2 };

	task_scheduler<
		decltype(ntc3),
		100,
		bool>::task_scheduler_impl<&decltype(ntc3)::update>
		task_ntc3{ ntc3 };
}

core::array<float, 3> samples{};

void setup() {
	lcd.begin();
	lcd.set_cursor(0, 0);
	lcd.write("Min");
	lcd.set_cursor(5, 0);
	lcd.write("Mid");
	lcd.set_cursor(10, 0);
	lcd.write("Max");
}

void loop() {

	auto uptime = millis();
	if(task_ntc1.exec(uptime)) samples[0] = ntc1.value<uno::placement::down>();
	if(task_ntc2.exec(uptime)) samples[1] = ntc2.value<uno::placement::down>();
	if(task_ntc3.exec(uptime)) samples[2] = ntc3.value<uno::placement::down>();

	const auto min = core::fold(samples.cbegin(), samples.cend(), samples[0], core::min{});
	const auto mid = core::fold(samples.cbegin(), samples.cend(), 0.0f, core::plus{}) / samples.size();
	const auto max = core::fold(samples.cbegin(), samples.cend(), samples[0], core::max{});

	lcd.set_cursor(0, 1);
	lcd.write(static_cast<int8_t>(min), 3);
	lcd.set_cursor(5, 1);
	lcd.write(static_cast<int8_t>(mid), 3);
	lcd.set_cursor(10, 1);
	lcd.write(static_cast<int8_t>(max), 3);
}
