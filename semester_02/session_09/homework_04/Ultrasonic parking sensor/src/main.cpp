#include <Arduino.h>
#include <math.h>
#include <uno.h>

/*
 * Разработайте программу, которая измеряет расстояние до препятствия с
 * периодичностью в 100 мс. Информация об измеренной дальности должна
 * отображаться на жидкокристаллическом индикаторе в метрах с точностью
 * до десятых долей.
 *
 * Если расстояние находится в диапазоне от 2 до 4 метров, то RGB-светодиод
 * должен непрерывно светиться.
 *
 * Если расстояние находится в диапазоне от 1 до 2 метров, то RGB-светодиод
 * должен моргать жёлтым светом с периодичностью 500 мс.
 *
 * Если расстояние менее 1 метра, то RGB-светодиод должен моргать красным
 * светом с периодичностью 100 мс.
 */

namespace {
	/// Объекты
	using namespace uno;
	using reg = hd44780::registers;
	liquid_crystal_display<
		bind<12, reg::rs>, bind<11, reg::e>,
		bind<10, reg::d0>, bind<9, reg::d1>, bind<8, reg::d2>, bind<7, reg::d3>,
		bind<6, reg::d4>, bind<5, reg::d5>, bind<4, reg::d6>, bind<3, reg::d7>
	> display{};

	light_emitting_diode<digital_signal, common_anode,
		bind<A2, color::red>, bind<A1, color::green>, bind<A0, color::blue>
	> led{};

	thermistor<A3,
		math::temperature_calculation<3950>,
		math::ema<analog_signal::value_type, math::real_number<55, -2>>
	> ntc{};

	using ch = hc_sr04_channels;
	using Trig = core::package_of_types<bind<A5, ch::trig>>;
	using Echo = core::package_of_types<bind<A4, ch::echo>>;
	ultrasonic_distance_sensor<
		Trig,
		Echo
	> parking_sensor{};

	/// Задачи
	using namespace control;
	task_scheduler<decltype(led), void, uint32_t>::task_scheduler_impl<&decltype(led)::blink> task_led{ led };
	task_scheduler<decltype(ntc), bool>::task_scheduler_impl<&decltype(ntc)::update> task_ntc{ ntc };
	task_scheduler<decltype(parking_sensor), void, float>::task_scheduler_impl<&decltype(parking_sensor)::update> task_parking_sensor{ parking_sensor };
}

namespace fox {
	bool equal(float lhs, float rhs, float eps = 1e-6f);
	bool less(float lhs, float rhs, float eps = 1e-6f);
	bool less_or_equal(float lhs, float rhs, float eps = 1e-6f);
	bool greater(float lhs, float rhs, float eps = 1e-6f);
	bool greater_or_equal(float lhs, float rhs, float eps = 1e-6f);
}

void setup() {
	display.begin();
	led.begin();
	parking_sensor.begin();

	display.write("RANGE:");
}

void loop() {
	display.set_cursor(5, 0);   /// Начальное положение курсора.
	float temperature{ 14.0F }; /// Начальное значение, использую для калибровки 14.0.
	uint16_t poll_time{ 100 };  /// Периодичность опроса парктроника.

	const auto uptime = millis();
	/* if(task_ntc.exec(uptime)) temperature = ntc.value<uno::placement::down>(); */
	task_parking_sensor.exec(uptime, poll_time, temperature);

	const auto distance = parking_sensor.value();
	display.write(distance, 2, 5);

	if(fox::less_or_equal(distance, 4.0f) && fox::greater(distance, 2.0f)) {
		using namespace uno;
		led.change_color(static_cast<uint32_t>(color::green));
	} else {
		core::pair<color, uint16_t> state{ color::red, 100 };
		if(fox::less_or_equal(distance, 2.0f) && fox::greater(distance, 1.0f)) {
			state.first = state.first | color::green;
			state.second = 500U;
		}
		task_led.exec(millis(), state.second, static_cast<uint32_t>(state.first));
	}
}

namespace fox {
	bool equal(float lhs, float rhs, float eps) {
		return abs(lhs - rhs) < eps;
	}

	bool less(float lhs, float rhs, float eps) { return (rhs - lhs) > eps; }
	bool less_or_equal(float lhs, float rhs, float eps) {
		return less(lhs, rhs) || equal(lhs, rhs);
	}

	bool greater(float lhs, float rhs, float eps) { return (lhs - rhs) > eps; }
	bool greater_or_equal(float lhs, float rhs, float eps) {
		return greater(lhs, rhs) || equal(lhs, rhs);
	}
}
