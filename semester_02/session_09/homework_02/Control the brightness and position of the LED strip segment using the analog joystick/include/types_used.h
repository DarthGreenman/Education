// types_used.h

#ifndef TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT
#define TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <Arduino.h>

namespace wokwi
{
	struct signal_digital
	{
		enum
		{
			low,
			high
		};
	};
	struct signal_analog
	{
		enum
		{
			low,
			high = 1023
		};
	};

	enum class signal_direction : uint8_t
	{
		input = INPUT,
		input_pullup = INPUT_PULLUP,
		output = OUTPUT
	};
	enum class rgb_colour : uint8_t
	{
		red,
		green,
		blue
	};
} // namespace wokwi

#endif // !TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT
