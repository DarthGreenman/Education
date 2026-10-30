/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Драйвер датчика движения на основе пироприемника.
 *
 */

#ifndef MOTION_SENSOR_H__FOR_THE_ARDUINO_UNO_PROJECT
#define MOTION_SENSOR_H__FOR_THE_ARDUINO_UNO_PROJECT

#include <electronic_component.h>
#include <common.h>
#include <basic_types.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {

	template<uint8_t Pin>
	class motion_sensor : public electronic_component<digital_signal, output_signal, Pin> {
		using basic_component = electronic_component<digital_signal, input_signal, Pin>;
	public:
		using signal_type = typename basic_component::signal_type;
		static constexpr uint8_t default_polling_time = 5; /** < Рекомендуемое значение частоты вызова планировщика. */

		constexpr motion_sensor() = default;
		motion_sensor(const motion_sensor&) = delete;
		motion_sensor(motion_sensor&&) = delete;
		~motion_sensor() = default;

		motion_sensor& operator=(const motion_sensor&) = delete;
		motion_sensor& operator=(motion_sensor&&) = delete;

		void begin() { basic_component::begin(); }

		[[nodiscard]] FORCEINLINE typename signal_type::value_type update() {
			return basic_component::read();
		}
	};

} /// !namespace uno

#endif /// !defined(MOTION_SENSOR_H__FOR_THE_ARDUINO_UNO_PROJECT)
