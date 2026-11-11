/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Драйвер ультразвукового датчика расстояния HC-SR04.
 *
 */

#ifndef ULTRASONIC_DISTANCE_SENSOR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define ULTRASONIC_DISTANCE_SENSOR_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <addons.h>
#include <algorithms.h>
#include <basic_types.h>
#include <callbacks.h>
#include <common.h>
#include <electronic_component.h>
#include <fixed_point.h>
#include <meta.h>

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
	enum class measure_unit : uint32_t {
		mm = 1'000,
		cm = 100,
		m = 1
	};

	namespace event {
		/** @brief Внешние события. */
		enum class motion : uint8_t {
			approach, /**< Приближение. */
			recede    /**< Удаление. */
		};
	} /// !namespace event

	enum class
		hc_sr04_channels : uint8_t {
		echo = 0b0000'0010, trig = 0b0000'0001
	};

	template <typename T, typename E>
	class ultrasonic_distance_sensor;

	template <
		typename... Ts,
		typename... Es
	>
	class ultrasonic_distance_sensor<core::package_of_types<Ts...>, core::package_of_types<Es...>> :
		public electronic_component<digital_signal, output_signal, get<0, Ts>::value...>,
		public electronic_component<digital_signal, input_signal, get<0, Es>::value...> {

		static_assert(sizeof...(Ts) == 1 && sizeof...(Es) == 1, "The number of contacts is incorrect, "
			"there should be one.");
		/// Проверяет корректность привязок.
		static_assert((is_valid_bindings_v<Ts, hc_sr04_channels::trig> && ...), "Wrong binding.");
		static_assert((is_valid_bindings_v<Es, hc_sr04_channels::echo> && ...), "Wrong binding.");

		using trig_component = electronic_component<digital_signal, output_signal, get<0, Ts>::value...>;
		using echo_component = electronic_component<digital_signal, input_signal, get<0, Es>::value...>;

		using event = event::motion;
		using pointer_to_callback_function =
			typename control::table_of_callback_functions<event>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = control::table_of_callback_functions<event>;

	public:
		using value_type = core::fixed_point<32, 8, core::signedness::unsigned_type>;
		using signal_type = digital_signal;

		constexpr ultrasonic_distance_sensor() = default;
		ultrasonic_distance_sensor(const ultrasonic_distance_sensor&) = delete;
		ultrasonic_distance_sensor(ultrasonic_distance_sensor&&) = delete;
		~ultrasonic_distance_sensor() = default;

		ultrasonic_distance_sensor& operator=(const ultrasonic_distance_sensor&) = delete;
		ultrasonic_distance_sensor& operator=(ultrasonic_distance_sensor&&) = delete;

		/** @brief Инициализирует состояние. */
		void begin() {
			trig_component::begin();
			echo_component::begin();
		}

		/**
		 * @brief Регистрирует обработчик события "Приближение".
		 * @param [in] action Callback-функция, вызываемая при нажатии
		 */
		FORCEINLINE void on_approach(const pointer_to_callback_function action) {
			_handlers.add(event::approach, action);
		}
		/**
		 * @brief Регистрирует обработчик события "Удаление".
		 * @param [in] action Callback-функция, вызываемая при отпускании
		 */
		FORCEINLINE void on_recede(const pointer_to_callback_function action) {
			_handlers.add(event::recede, action);
		}

		/** @brief Обновляет состояние. */
		FORCEINLINE void update(float temperature) {
			trig_component::write(signal_type::low);
			delayMicroseconds(2);
			trig_component::write(signal_type::high);
			delayMicroseconds(10);
			trig_component::write(signal_type::low);

			auto range_calc = [this, temperature]
			(typename signal_type::value_type sample, uint32_t timeot) noexcept {
				/*
				 * Функция math::sonic_speed(float) возвращает значение скорости
				 * звука в типе float.
				 * Значение приводится к типу с фиксированной точкой, той же
				 * размерности, что и поле _range.			 *
				 */
				const value_type sonic_speed{ math::sonic_speed(temperature) };
				/*
				 * Коэффициент учитывает:
				 *    приведения к размерности - 1'000'000
				 *    сигнал затачивает времени в 2 раза больше, чем необходимо
				 *    для рассчета расстояния
				 */
				constexpr auto reduction_ratio = 2'000'000UL;
				return sonic_speed * pulse_duration(sample, timeot) / reduction_ratio;
			};

			constexpr value_type threshold{ 0.01f }; /**< Чувствительность датчика. */
			auto callback = [this](event e) noexcept {
				if(const auto function = _handlers.call(e); function) { function(); }
			};

			if(const auto range = range_calc(signal_type::high, 30'000UL);
				range + threshold < _range) {
				_range = range;
				callback(event::approach);
			} else if(range - threshold > _range) {
				_range = range;
				callback(event::recede);
			}
		}

		/**
		 * @brief Возвращает значение расстояния до объекта.
		 * @tparam Unit идентификатор единиц измерения
		 * @return      значение в мм
		 */
		template<measure_unit Unit>
		[[nodiscard]] FORCEINLINE core::enable_if_t<
			static_cast<uint32_t>(Unit) == static_cast<uint32_t>(measure_unit::mm), 
			uint16_t> value() const noexcept {
				return static_cast<int16_t>(_range.restore() * static_cast<uint32_t>(Unit)); 
			}

		/**
		 * @brief Возвращает значение расстояния до объекта.
		 * @tparam Unit идентификатор единиц измерения
		 * @return      значение в см, м
		 */
		template<measure_unit Unit = measure_unit::m>
		[[nodiscard]] FORCEINLINE core::enable_if_t<
			static_cast<uint32_t>(Unit) != static_cast<uint32_t>(measure_unit::mm), 
			float> value() const noexcept { return _range.restore(); }

	private:
		/**
		 * @brief Расчитывает время, необходимое для прохождения сигналом пути
		 *        от источника до предмета и обратно.
		 *
		 * @param [in] sample  значение испускаемого сигнала
		 * @param [in] timeout интервалы между сигналами
		 * @return             время в микросекндах
		 */
		auto pulse_duration(typename signal_type::value_type sample, uint32_t timeout) {
			auto wait = [timeout](uint32_t point_of_time) {
				return (micros() - point_of_time >= timeout)
					? false : true;
			};

			const auto waiting_start_time = micros();
			while(echo_component::read() != sample) {
				if(!wait(waiting_start_time)) {
					return core::pure_type_t<decltype(micros())>{};
				}
			}

			const auto receiving_start_time = micros();
			while(echo_component::read() == sample) {
				if(!wait(receiving_start_time)) {
					return core::pure_type_t<decltype(micros())>{};
				}
			}
			return micros() - receiving_start_time;
		}

		value_type _range{};
		table_of_pointers_to_callback_function _handlers{}; /**< Таблица колбэков(размер мин. 8 байт). */
	};

} /// !namespace uno

#endif /// !defined(ULTRASONIC_DISTANCE_SENSOR_H_FOR_THE_ARDUINO_UNO_PROJECT)

