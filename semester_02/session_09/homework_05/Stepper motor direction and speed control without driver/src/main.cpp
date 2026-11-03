#include <Arduino.h>
#include <Stepper.h>
#include <uno.h>

/*
 * Соберите в симуляторе Wokwi схему, состоящую из:
 *  платы Arduino UNO;
 *  трёх кнопок;
 *  потенциометра;
 *  шагового двигателя.
 * Подключите выход потенциометра к аналоговому входу платы Arduino UNO,
 * а кнопки и шаговый двигатель — к оставшимся свободным выводам.
 * Разработайте программу, которая управляет запуском и остановом двигателя
 * по нажатию на одну кнопку «Старт/стоп».
 * Для переключения направления вращения использовать две независимые
 * кнопки — «Влево», «Вправо». Реализовать программное подавление дребезга
 * контактов кнопок.
 * Скорость вращения регулировать с помощью потенциометра.
*/

constexpr uint16_t number_of_steps = 200; /**< Количество шагов на один оборот двигателя */

namespace drv {
	/// Объекты
	Stepper stepper_motor(number_of_steps, 9, 10, 11, 12);

	using namespace uno;
	light_emitting_diode<digital_signal, bind<5>> led{};

	button<input_signal, 8, math::integrator<typename analog_signal::value_type>> b_ounterclockwise{};
	button<input_signal, 6, math::integrator<typename analog_signal::value_type>> b_clockwise{};
	button<input_signal, 7, math::integrator<typename analog_signal::value_type>> b_on_off{};
	variable_resistor<A0, math::ema<typename analog_signal::value_type, math::real_number<75, -2>>> p_speed{};

	/// Задачи
	using namespace control;
	/// Мотор
	task_scheduler<
		core::pure_type_t<decltype(stepper_motor)>, void, int>::task_scheduler_impl<
		&core::pure_type_t<decltype(stepper_motor)>::step
		> task_stepper_motor{ stepper_motor };

	/// Кнопки
	task_scheduler<
		core::pure_type_t<decltype(b_ounterclockwise)>, void>::task_scheduler_impl<
		&core::pure_type_t<decltype(b_ounterclockwise)>::update
		> task_ounterclockwise{ b_ounterclockwise };

	task_scheduler<
		core::pure_type_t<decltype(b_clockwise)>, void>::task_scheduler_impl<
		&core::pure_type_t<decltype(b_clockwise)>::update
		> task_clockwise{ b_clockwise };

	task_scheduler<
		core::pure_type_t<decltype(b_on_off)>, void>::task_scheduler_impl<
		&core::pure_type_t<decltype(b_on_off)>::update
		> task_on_off{ b_on_off };

	/// Переменный резистр
	task_scheduler<
		core::pure_type_t<decltype(p_speed)>, void>::task_scheduler_impl<
		&core::pure_type_t<decltype(p_speed)>::update
		> task_speed{ p_speed };
}

bool is_running{}; /**< Флаг опредляющий выполнять вращение ротора двигателя, или нет. */

enum class direction_of_rotation : int8_t { counterclockwise = -1, clockwise = 1 }; /**< Возможные направления вращения ротора. */
auto curr_direction = direction_of_rotation::clockwise; /**< Текщее направление вращения. */

enum class rotation_speed : uint16_t { slow = 10, fast = 150 }; /**< Пределы частот вращения ротора [об / мин]. */
auto time_between_steps_calc = [](rotation_speed value) { return 60'000 / number_of_steps / static_cast<uint16_t>(value); };
auto time_between_steps = time_between_steps_calc(rotation_speed::slow); /**< Интервал между шагами. */

#define DEBUG_TASK

void setup() {
#ifdef DEBUG_TASK
	Serial.begin(115200);
#endif

	drv::led.begin();
	drv::b_ounterclockwise.begin();
	drv::b_clockwise.begin();
	drv::b_on_off.begin();

	/// Обработчики событий

	/**< Включить / выключить. */
	drv::b_on_off.on_pressed([]() { (is_running = !is_running) ? drv::led.on() : drv::led.off(); });
	/**< Установить вращение против часовой стрелки. */
	drv::b_ounterclockwise.on_pressed([]() { curr_direction = direction_of_rotation::counterclockwise; });
	/**< Установить вращение по часовой стрелке. */
	drv::b_clockwise.on_pressed([]() { curr_direction = direction_of_rotation::clockwise; });
	/**< Изменить количество оборотов. */
	drv::p_speed.on_change([]() {
		using signal_type = core::pure_type_t<decltype(drv::p_speed)>::signal_type;
		time_between_steps = map(
			drv::p_speed.value(),
			signal_type::low,
			signal_type::high,
			time_between_steps_calc(rotation_speed::slow),
			time_between_steps_calc(rotation_speed::fast)
		);
#ifdef DEBUG_TASK
		Serial.print("Rotation speed: ");
		Serial.print(60'000 / number_of_steps / time_between_steps);
		Serial.println(" rpm");
#endif
	});

}

void loop() {
	const auto uptime = millis();
	constexpr uint8_t polling_time{ 5 };

	drv::task_ounterclockwise.exec(uptime, polling_time);
	drv::task_clockwise.exec(uptime, polling_time);
	drv::task_on_off.exec(uptime, polling_time);
	drv::task_speed.exec(uptime, polling_time);

	if(is_running) {
		drv::task_stepper_motor.exec(uptime, time_between_steps, static_cast<int8_t>(curr_direction));
	}
}
