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
#include <basic_types.h>
#include <common.h>
#include <electronic_component.h>
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
		/// Проверить корректность привязок.
		static_assert((is_valid_bindings_v<Ts, hc_sr04_channels::trig> && ...), "Wrong binding.");
		static_assert((is_valid_bindings_v<Es, hc_sr04_channels::echo> && ...), "Wrong binding.");

	public:
		using signal_type = digital_signal;
		using trig_component = electronic_component<digital_signal, output_signal, get<0, Ts>::value...>;
		using echo_component = electronic_component<digital_signal, input_signal, get<0, Es>::value...>;

		constexpr ultrasonic_distance_sensor() = default;
		ultrasonic_distance_sensor(const ultrasonic_distance_sensor&) = delete;
		ultrasonic_distance_sensor(ultrasonic_distance_sensor&&) = delete;
		~ultrasonic_distance_sensor() = default;

		ultrasonic_distance_sensor& operator=(const ultrasonic_distance_sensor&) = delete;
		ultrasonic_distance_sensor& operator=(ultrasonic_distance_sensor&&) = delete;

		void begin() {
			trig_component::begin();
			echo_component::begin();
		}

		FORCEINLINE void update(float temperature) noexcept {

			trig_component::write(signal_type::low);
			delayMicroseconds(2);
			trig_component::write(signal_type::high);
			delayMicroseconds(10);
			trig_component::write(signal_type::low);

			const auto sonic_speed = 331.5f + 0.6f * temperature;
			_distance = sonic_speed *
				pulse_duration(signal_type::high, 30'000UL) / (1'000'000 * 2);
		}

		[[nodiscard]] FORCEINLINE auto value() const noexcept { return _distance; }

	private:
		auto pulse_duration(typename signal_type::value_type sample, uint32_t timeout) {
			auto wait = [timeout](uint32_t point_of_time) {
				return (micros() - point_of_time >= timeout)
					? false : true;
			};
			for(const auto start_of_waiting_time = micros();
				echo_component::read() != sample;) {
				if(!wait(start_of_waiting_time))
					return decltype(micros()){0};
			}

			const auto start_of_receiving_pulse = micros();
			while(echo_component::read() == sample) {
				if(!wait(start_of_receiving_pulse))
					return decltype(micros()){0};
			}

			return micros() - start_of_receiving_pulse;
		}

		float _distance{};
	};

} /// !namespace uno

#endif /// !defined(ULTRASONIC_DISTANCE_SENSOR_H_FOR_THE_ARDUINO_UNO_PROJECT)
