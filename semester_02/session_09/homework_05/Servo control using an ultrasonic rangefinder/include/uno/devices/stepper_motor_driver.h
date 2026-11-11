/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Драйвер, для работы шагового двигателя в блокирующем режиме.
 *
 */
#ifndef STEPPER_MOTOR_DRIVER_H_FOR_THE_ARDUINO_UNO_PROJECT
#define STEPPER_MOTOR_DRIVER_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <addons.h>
#include <basic_types.h>
#include <common.h>
#include <electronic_component.h>
#include <meta.h>
#include <numeric_limits.h>
#include <type_traits.h>

#ifdef HAL
#include <arduino_time.h>
#else
#include <Arduino.h>
#endif

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif


namespace uno {
	/// Направление вращения ротора.
	enum class direction_of_rotation : uint8_t {
		counterclockwise,
		clockwise
	};

	/// Каналы драйвера A4988.
	enum class a4988_channels : uint8_t {
		enable = 0b1000'0000, /**< Канал управления питанием шагового двигателя. */
		step = 0b0100'0000,   /**< Канал управления шагами. */
		dir = 0b0010'0000,    /**< Канал управления направлением вращения ротора. */
		ms1 = 0b0000'0100,    /**< Канал конфигурации микрошагов 1. */
		ms2 = 0b0000'0010,    /**< Канал конфигурации микрошагов 2. */
		ms3 = 0b0000'0001     /**< Канал конфигурации микрошагов 3. */
	};

	/// Операторы для перечислений, которые позволяют использовать их значения
	/// в битовых операциях без необходимости явного приведения типов.
	FORCEINLINE constexpr auto operator|(a4988_channels lhs, a4988_channels rhs) noexcept {
		return static_cast<a4988_channels>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	/// Конфигурации микрошагов для драйвера A4988.
	enum class microstepping : uint8_t {
		full = 0b000'0000, /**< Полный шаг. */
		half = static_cast<uint8_t>(a4988_channels::ms1), /**< Половинный шаг. */
		quarter = static_cast<uint8_t>(a4988_channels::ms2), /**< Четверть шага. */
		eighth = static_cast<uint8_t>(a4988_channels::ms1 | a4988_channels::ms2), /**< Восьмая часть шага. */
		sixteenth = static_cast<uint8_t>(a4988_channels::ms1 | a4988_channels::ms2 | a4988_channels::ms3) /**< Шестнадцатая часть шага. */
	};

	/**
	 * @brief Класс драйвера шагового двигателя, для работы шагового двигателя
	 *        в блокирующем режиме.
	 *
	 * Реализует управление шаговым двигателем с помощью драйвера A4988. Включает
	 * методы для настройки скорости вращения, выполнения заданного числа шагов и
	 * управления режимом микрошагов.
	 *
	 * @tparam Bs Параметры привязки каналов драйвера A4988.
	 */
	template <auto N, typename... Bs>
	class stepper_motor_driver :
		public electronic_component<digital_signal, output_signal, get<0, Bs>::value...> {

		/// Проверяет, что количество шагов является целым числом и больше нуля
		static_assert(core::is_arithmetic_integer_v<decltype(N)>, "The number of steps must be an integer type.");
		static_assert(N > 0, "The number of steps must be greater than zero.");
		/// Проверяет количество подключаемых каналов равно 2, 5 или 6.
		static_assert(sizeof...(Bs) == 2 || sizeof...(Bs) == 5 || sizeof...(Bs) == 6,
			"The number of contacts is incorrect, there should be two, or five, or six.");
		/// Проверяет уникальность привязок.
		static_assert(core::is_unique_types_v<Bs...>, "The bindings are not unique.");
		/// Проверяет корректность привязок.
		using ch = a4988_channels;
		static_assert((is_valid_bindings_v<Bs, ch::enable, ch::ms1, ch::ms2, ch::ms3,
			ch::step, ch::dir> && ...), "Wrong binding.");
		using basic_component = electronic_component<digital_signal, output_signal, get<0, Bs>::value...>;

	public:
		using signal_type = typename basic_component::signal_type;
		static constexpr auto number_of_steps = N; /**< Количество шагов на один оборот ротора. */
		
		constexpr stepper_motor_driver() = default;
		stepper_motor_driver(const stepper_motor_driver&) = delete;
		stepper_motor_driver(stepper_motor_driver&&) = delete;
		~stepper_motor_driver() = default;

		stepper_motor_driver& operator=(const stepper_motor_driver&) = delete;
		stepper_motor_driver& operator=(stepper_motor_driver&&) = delete;

		/**
		 * @brief Инициализирует состояние.
		 *
		 * Настраивает подключённые выводы как выходы и переводит драйвер в
		 * исходное состояние, при котором питание шагового двигателя отключено.
		 */
		void begin() {
			basic_component::begin();

			using enable_type = find_bind_t<ch::enable, Bs...>; /**< Тип привязки канала enable. */
			if constexpr(!core::is_same_v<enable_type, void>) { /**< Проверяет, что канал управления питанием подключен. */
				basic_component::write(signal_type::high, get<0, enable_type>::value); /**< Отключает питание шагового двигателя. */
			}
		}

		/** @brief Включает питание. */
		FORCEINLINE void on() {
			using enable_type = find_bind_t<ch::enable, Bs...>; /**< Тип привязки канала enable. */
			/// Проверяет, что канал управления питанием подключен.
			static_assert(!core::is_same_v<enable_type, void>, "To use the functionality, "
				"you need to connect a power management pin.");
			basic_component::write(signal_type::low, get<0, enable_type>::value);
		}

		/** @brief Отключает питание. */
		FORCEINLINE void off() {
			using enable_type = find_bind_t<ch::enable, Bs...>; /**< Тип привязки канала enable. */
			/// Проверяет, что канал управления питанием подключен.
			static_assert(!core::is_same_v<enable_type, void>, "To use the functionality, "
				"you need to connect a power management pin.");
			basic_component::write(signal_type::high, get<0, enable_type>::value);
		}

		/**
		 * @brief Настраивает режим микрошагов.
		 *
		 * Данный метод позволяет настроить режим микрошагов для шагового
		 * двигателя, управляемого драйвером A4988. Конфигурация микрошагов
		 * определяется значением параметра `config`, который представляет
		 * собой битовую маску, указывающую состояние каналов ms1, ms2 и ms3.
		 * Каждый бит в `config` соответствует одному из каналов микрошагов:
		 * - (0b00000100) - канал ms1
		 * - (0b00000010) - канал ms2
		 * - (0b00000001) - канал ms3
		 *
		 * @param config Конфигурация микрошагов.
		 */
		FORCEINLINE void microstep(microstepping config) {
			using ms1_type = find_bind_t<ch::ms1, Bs...>; /**< Тип привязки канала ms1. */
			using ms2_type = find_bind_t<ch::ms2, Bs...>; /**< Тип привязки канала ms2. */
			using ms3_type = find_bind_t<ch::ms3, Bs...>; /**< Тип привязки канала ms3. */

			/// Проверяет, что все каналы конфигурации микрошагов подключены.
			static_assert(!core::is_same_v<ms1_type, void> && !core::is_same_v<ms2_type, void> &&
				!core::is_same_v<ms3_type, void>, "To use the functionality, you need to connect "
				"the configuration pins with a microstep.");

			/// Конфигурирует каналы микрошагов в соответствии с переданным значением.
			auto cast = [](uint8_t value) { return (value & 0x01) ? signal_type::high : signal_type::low; };

			basic_component::write(cast((static_cast<uint8_t>(config) >> 2)), get<0, ms1_type>::value);
			basic_component::write(cast((static_cast<uint8_t>(config) >> 1)), get<0, ms2_type>::value);
			basic_component::write(cast(static_cast<uint8_t>(config)), get<0, ms3_type>::value);

			/// Настраивает множитель количества шагов в зависимости 
			/// от конфигурации микрошагов.
			multiplier(config);
		}

		/**
		 * @brief Настраивает скорость вращения ротора.

		 * @param [in] rpm Скорость вращения ротора в оборотах в минуту.
		 */
		FORCEINLINE void speed(uint16_t rpm) noexcept {
			_rpm = rpm; /**< Сохраняет скорость вращения ротора в оборотах в минуту. */
			speed_update(); /**< Вычисляет интервал между шагами в микросекундах. */
		}

		/**
		 * @brief Выполняет заданное количество шагов.
		 *
		 * @param [in] spin  Направление вращения ротора.
		 * @param [in] steps Количество шагов.
		 */
		FORCEINLINE void step(direction_of_rotation spin, uint16_t steps = 1) {
			/// Проверяет, что интервал между шагами не равен максимальному значению.
			if(_time_between_steps == (core::numeric_limits<decltype(_time_between_steps)>::max)())
				return;

			_point_of_time = micros(); /**< Инициализирует время работы в микросекундах. */
			basic_component::write(spin == direction_of_rotation::clockwise ? signal_type::high : signal_type::low,
				get<0, find_bind_t<a4988_channels::dir, Bs...>>::value); /**< Настраивает направление вращения ротора. */
			using wider_type = core::make_wider_type_t<core::pure_type_t<decltype(steps)>>; /**< Более широкий тип для хранения количества микрошагов. */
			const auto microsteps = static_cast<wider_type>(steps) * _multiplier; /**< Количество микрошагов, необходимых для выполнения заданного числа шагов. */

			for(core::pure_type_t<decltype(microsteps)> step{}; step < microsteps; ) {
				if(do_step()) /**< Проверяет, завершён ли текущий шаг. */
					++step; /**< Увеличивает счётчик выполненных микрошагов. */
			}
		}

	private:
		/**
		 * @brief Вычисляет интервал между шагами в микросекундах.
		 *
		 * Интервал между шагами вычисляется на основе заданной скорости вращения
		 * ротора и количества шагов на один оборот. Если скорость вращения
		 * равна нулю, интервал устанавливается в максимальное значение.
		 */
		FORCEINLINE void speed_update() noexcept {
			using T = core::pure_type_t<decltype(_time_between_steps)>;
			_time_between_steps = !_rpm
				? (core::numeric_limits<T>::max)()
				: static_cast<T>(60'000'000) / number_of_steps / _multiplier / _rpm;
		}

		/**
		 * @brief Выполняет один шаг.
		 *
		 * Проверяет, прошло ли достаточно времени с момента последнего шага,
		 * и если да, то генерирует импульс на канале step для выполнения шага.
		 *
		 * @attention Ширина импульса ≥ 1 мкс обеспечивается временем вызова write();
		 *            при замене на портовый доступ добавить delayMicroseconds(1).
		 *
		 * @return @c true, если шаг завершён; @c false, если шаг ещё не завершён.
		 */
		FORCEINLINE bool do_step() {
			using step_type = find_bind_t<a4988_channels::step, Bs...>; /**< Тип привязки канала step. */
			if(const auto uptime = micros(); uptime - _point_of_time >= _time_between_steps) {
				_point_of_time += _time_between_steps; /**< Обновляет время работы. */
				basic_component::write(signal_type::high, get<0, step_type>::value); /**< Генерирует импульс на канале step для выполнения шага. */
				basic_component::write(signal_type::low, get<0, step_type>::value); /**< Завершает импульс на канале step. */
				return true; /**< Импульс сгенерирован, микрошаг выполнен в этом вызове. */
			}
			return false; /**< Шаг ещё не завершён. */
		}

		/**
		 * @brief Настраивает множитель количества шагов в зависимости от
		 *        конфигурации микрошагов.
		 *
		 * @param [in] config Конфигурация микрошагов.
		 */
		FORCEINLINE void multiplier(microstepping config) noexcept {
			switch(config) {
				case microstepping::full:
					_multiplier = 1;
					break;
				case microstepping::half:
					_multiplier = 2;
					break;
				case microstepping::quarter:
					_multiplier = 4;
					break;
				case microstepping::eighth:
					_multiplier = 8;
					break;
				case microstepping::sixteenth:
					_multiplier = 16;
					break;
				default:
					break;
				speed_update(); /**< Обновляет интервал между шагами в микросекундах. */
			}
		}

		core::pure_type_t<decltype(micros())> _time_between_steps{ (core::numeric_limits<uint32_t>::max)() }; /**< Интервал между шагами. */
		core::pure_type_t<decltype(micros())> _point_of_time{}; /**< Время работы в микросекундах. */
		uint16_t _rpm{}; /**< Скорость вращения ротора в оборотах в минуту. */
		uint8_t _multiplier{ 1 }; /**< Множитель количества шагов в зависимости от конфигурации микрошагов. */
	};

} /// !namespace uno

#endif /// !defined(STEPPER_MOTOR_DRIVER_H_FOR_THE_ARDUINO_UNO_PROJECT)

