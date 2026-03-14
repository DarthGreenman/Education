// hardware.h

#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>
#include <assert.h>
#include <stdint.h>

namespace wokwi
{
enum class signal_type : uint8_t
{
	digital,
	analog
}; // enum class signal_type

enum class signal_direction : uint8_t
{
	input = INPUT,
	output = OUTPUT
}; // enum class signal_direction

enum class boundary_values_of_analog_signal : uint16_t
{
	low,
	high = 1023
}; // enum class boundary_values_of_analog_signal

enum digital_signal_values : uint8_t
{
	low,
	high
}; // enum digital_signal_values : uint8_t

struct channel_connect_params
{
	uint8_t pin;
	signal_type type;
	signal_direction direction;
}; // struct channel_connect_params

template <uint8_t Number_of_channels> class electronic_component
{
protected:
	static constexpr auto number_of_channels = Number_of_channels;

	constexpr electronic_component() = default;
	explicit electronic_component(const wokwi::channel_connect_params (&channels)[Number_of_channels]);
	explicit electronic_component(uint8_t pin, wokwi::signal_type type, wokwi::signal_direction direction);
	electronic_component(const electronic_component&) = default;
	electronic_component(electronic_component&& movable) = default;
	virtual ~electronic_component() = default;

	electronic_component& operator=(const electronic_component&) = delete;
	electronic_component& operator=(electronic_component&&) = delete;

	void set_params(uint8_t pin, wokwi::signal_type type, wokwi::signal_direction direction);
	void write(uint8_t channel_number, digital_signal_values signal_value);
	void write(uint8_t channel_number, uint16_t signal_value);
	uint16_t read(uint8_t channel_number) const;

private:
	static bool is_valid_pin_params(uint8_t pin, wokwi::signal_type type, wokwi::signal_direction direction);
	const wokwi::channel_connect_params& get_params(uint8_t channel_number) const;

	struct channel_params
	{
		channel_connect_params params; // pin, signal_type, signal_direction
		int signal_value;			   // 0 to 1023 for analog, 0 or 1 for digital
	};
	channel_params _channels[Number_of_channels]{};
	uint8_t _count{};
}; // template<uint8_t Number_of_channels> class electronic_component

template <uint8_t Number_of_channels>
inline electronic_component<Number_of_channels>::electronic_component(const wokwi::channel_connect_params (&channels)[Number_of_channels])
{
	for (auto&& ch : channels)
		set_params(ch.pin, ch.type, ch.direction);
}

template <uint8_t Number_of_channels>
inline electronic_component<Number_of_channels>::electronic_component(
	uint8_t pin, wokwi::signal_type type, wokwi::signal_direction direction)
	: electronic_component{{wokwi::channel_connect_params{pin, type, direction}}}
{
}

template <uint8_t Number_of_channels>
inline void electronic_component<Number_of_channels>::set_params(uint8_t pin, wokwi::signal_type type, wokwi::signal_direction direction)
{
	if (_count == Number_of_channels)
	{
		_count = 0;
		return;
	}
	assert(is_valid_pin_params(pin, type, direction) && "Incorrect channel parameters.");
	_channels[_count++] = channel_params{wokwi::channel_connect_params{pin, type, direction}, 0};
	pinMode(pin, static_cast<uint8_t>(direction));
}

template <uint8_t Number_of_channels>
inline void electronic_component<Number_of_channels>::write(uint8_t channel_number, digital_signal_values signal_value)
{
	assert(
		(signal_value == digital_signal_values::low || signal_value == digital_signal_values::high) && "Incorrect digital signal value.");
	const auto& ch = get_params(channel_number);
	assert(ch.type == wokwi::signal_type::digital && "The method is designed to process digital signals.");
	digitalWrite(ch.pin, static_cast<uint8_t>(signal_value));
}
template <uint8_t Number_of_channels>
inline void electronic_component<Number_of_channels>::write(uint8_t channel_number, uint16_t signal_value)
{
	assert(signal_value >= static_cast<uint32_t>(boundary_values_of_analog_signal::low) &&
		   signal_value <= static_cast<uint32_t>(boundary_values_of_analog_signal::high) && "Incorrect analog signal value.");
	const auto& ch = get_params(channel_number);
	assert(ch.type == wokwi::signal_type::analog && "The method is designed to process analog signals.");
	analogWrite(ch.pin, signal_value);
}

template <uint8_t Number_of_channels> inline uint16_t electronic_component<Number_of_channels>::read(uint8_t channel_number) const
{
	const auto& ch = get_params(channel_number);
	return ch.type == wokwi::signal_type::analog ? analogRead(ch.pin) : digitalRead(ch.pin);
}

template <uint8_t Number_of_channels>
inline bool electronic_component<Number_of_channels>::is_valid_pin_params(
	uint8_t pin, wokwi::signal_type signal_type, wokwi::signal_direction direction)
{
	switch (signal_type)
	{
	case wokwi::signal_type::analog:
		return direction == wokwi::signal_direction::input ? pin >= 14u && pin <= 19u
														   : pin == 3u || pin == 5u || pin == 6u || (pin >= 9u && pin <= 11u);
	case wokwi::signal_type::digital:
		return pin <= 13u;
	}
	return false;
}

template <uint8_t Number_of_channels>
inline const wokwi::channel_connect_params& electronic_component<Number_of_channels>::get_params(uint8_t channel_number) const
{
	assert(channel_number <= Number_of_channels && "Incorrect channel number value.");
	return _channels[channel_number].params;
}

} // namespace wokwi

#endif // HARDWARE_H