#include <Arduino.h>
#include <math.h>
#include <uno.h>

/*
 * Разработайте библиотеку, содержащую класс для управления шаговым двигателем
 * с помощью драйвера А4988.
 * Библиотека должна содержать конструктор, который задаёт выводы, используемые
 * для управления драйвером, и два метода:
 *  - задание скорости вращения;
 *  - перемещение на заданное число шагов.
 * Разработайте программу, которая показывает работу разработанной библиотеки:
 * медленный поворот на 360 градусов в одну сторону и быстрый поворот на
 * 360 градусов в обратную сторону.
 */

#define _DEBUG_MODE

namespace drv {
	using namespace uno;

	using ch = a4988_channels;
	stepper_motor_driver<
		200,
		bind<8, ch::enable>,
		bind<7, ch::ms1>,
		bind<6, ch::ms2>,
		bind<5, ch::ms3>,
		bind<3, ch::step>,
		bind<2, ch::dir>
	> motor{};
}
/// Количество шагов, необходимых для поворота ротора на 360 градусов.
constexpr auto steps = core::pure_type_t<decltype(drv::motor)>::number_of_steps;
enum class rotation_speed : uint16_t { slow = 10, fast = 200 }; /**< Пределы частот вращения ротора [об / мин]. */

#ifdef DEBUG_MODE
constexpr auto work_time = 60'000'000; /**< Время работы программы в микросекундах. */
#endif

void setup() {
	drv::motor.begin();
	drv::motor.on();
	drv::motor.microstep(uno::microstepping::sixteenth);
}

void loop() {
#ifdef DEBUG_MODE
	/// В режиме отладки программа работает в течение 60 секунд, после чего останавливается.
	if(work_time >= micros()) {
		drv::motor.speed(static_cast<uint16_t>(rotation_speed::fast));
		drv::motor.step(uno::direction_of_rotation::clockwise, steps);
	}
#else
	/// В обычном режиме программа выполняет поворот ротора на 360 градусов в одну сторону
	/// с медленной скоростью, затем на 360 градусов в обратную сторону с высокой скоростью.
	drv::motor.speed(static_cast<uint16_t>(rotation_speed::slow));
	drv::motor.step(uno::direction_of_rotation::clockwise, steps);
	delay(1000);

	drv::motor.speed(static_cast<uint16_t>(rotation_speed::fast));
	drv::motor.step(uno::direction_of_rotation::counterclockwise, steps);
	delay(1000);
#endif
}

