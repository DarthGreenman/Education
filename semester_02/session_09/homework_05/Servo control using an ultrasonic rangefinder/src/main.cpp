#include <Arduino.h>
#include <Servo.h>
#include <uno.h>

/*
 * Разработайте программу, в которой реализуется поворот сервопривода на угол,
 * обратно пропорциональный измеренной дальности, при максимальной дальности
 * угол 0 градусов, при минимальной — 180 градусов. Опрос дальномера проводить
 * с периодичностью 10 мс.
 */

namespace drv {
	/// Объекты
	Servo arm{};

	using namespace uno;
	using ch = hc_sr04_channels;
	using Trig = core::package_of_types<bind<A5, ch::trig>>;
	using Echo = core::package_of_types<bind<A4, ch::echo>>;
	ultrasonic_distance_sensor<
		Trig,
		Echo
	> rangefinder{};

	/// Задачи
	using namespace control;
	task_scheduler<
		decltype(rangefinder), void, float>::task_scheduler_impl<
		&decltype(rangefinder)::update
		> task_rangefinder{ rangefinder };
}

int16_t angle{}; /// Угол поворота.

#define _DEBUG_TASK	

void setup() {
#ifdef DEBUG_TASK
	Serial.begin(9600);
#endif
	drv::arm.attach(3);
	drv::rangefinder.begin();
	/// События: приближение и удаление
	drv::rangefinder.on_approach([]() { angle = map(drv::rangefinder.value<uno::measure_unit::mm>(), 3980, 20, 0, 180);	});
	drv::rangefinder.on_recede([]() { angle = map(drv::rangefinder.value<uno::measure_unit::mm>(), 20, 3980, 180, 0); });
}

void loop() {
	constexpr core::pure_type_t<decltype(millis())> poll_time{ 10 };  /// Периодичность опроса парктроника.
	constexpr float temperature{ 14.0F };

	drv::task_rangefinder.exec(millis(), poll_time, temperature);
	drv::arm.write(angle);

#ifdef DEBUG_TASK
	Serial.println(drv::rangefinder.value());
#endif

}

