// potentiometer.h

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "callbacks_table.h"
#include "hardware.h"
#include "helper.h"
#include "low_pass_filter.h"
#include "events.h"

#include <math.h>
#include <stdint.h>

namespace wokwi
{
template <typename Low_pass_filter> class potentiometer : public wokwi::electronic_component<1u>
{
public:
	using pointer_to_callback_function =
		typename wokwi::table_of_callback_functions<wokwi::potentiometer_events>::pointer_to_callback_function;
	using table_of_pointers_to_callback_function = wokwi::table_of_callback_functions<wokwi::potentiometer_events>;
	using event = wokwi::potentiometer_events;
	using basic = wokwi::electronic_component<1u>;

	potentiometer() = default;
	explicit potentiometer(const wokwi::channel_connect_params (&channel)[1u], int left_border, int right_border,
		Low_pass_filter low_pass_filter = Low_pass_filter{});
	potentiometer(const potentiometer&) = default;
	potentiometer(potentiometer&&) = default;
	~potentiometer() = default;

	potentiometer& operator=(const potentiometer&) = delete;
	potentiometer& operator=(potentiometer&&) = delete;

	void set_turning_callback(const pointer_to_callback_function& turn);
	float get_the_output_signal() const;
	void update();

private:
	table_of_pointers_to_callback_function _handler_table{};
	Low_pass_filter _low_pass_filter{};
	float _signal_value{};
	helper::couple<int, int> _boundaries_of_scale{};
	static constexpr uint8_t _channel_number{};
};

template <typename Low_pass_filter>
inline potentiometer<Low_pass_filter>::potentiometer(
	const wokwi::channel_connect_params (&channel)[1u], int left_border, int right_border, Low_pass_filter low_pass_filter)
	: wokwi::electronic_component<1u>(channel), _handler_table{}, _low_pass_filter{low_pass_filter}, _signal_value{},
	  _boundaries_of_scale{left_border, right_border}
{
	analogReference(DEFAULT);
}

template <typename Low_pass_filter>
inline void potentiometer<Low_pass_filter>::set_turning_callback(const pointer_to_callback_function& turn)
{
	_handler_table.add_handler(event::turn, turn);
}
template <typename Low_pass_filter> inline float potentiometer<Low_pass_filter>::get_the_output_signal() const
{
	return helper::map(_signal_value, static_cast<uint32_t>(wokwi::boundary_values_of_analog_signal::low),
		static_cast<uint32_t>(wokwi::boundary_values_of_analog_signal::high), _boundaries_of_scale.first, _boundaries_of_scale.second);
}
template <typename Low_pass_filter> inline void potentiometer<Low_pass_filter>::update()
{
	if (!_low_pass_filter(basic::read(_channel_number)))
		return;

	const auto signal_value = _low_pass_filter();
	if (abs(signal_value - _signal_value) < 0.01f)
		return;

	if (signal_value >= static_cast<uint32_t>(wokwi::boundary_values_of_analog_signal::low) &&
		signal_value <= static_cast<uint32_t>(wokwi::boundary_values_of_analog_signal::high))
	{
		if (_handler_table.handler(event::turn))
			_handler_table.handler(event::turn)();
		_signal_value = signal_value;
	}
}
// class potentiometer
} // namespace wokwi

#endif // POTENTIOMETER_H