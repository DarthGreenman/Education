// low_pass_filter.h

#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#include "helper.h"

#include <stdint.h>
#include <Arduino.h>

namespace wokwi
{
namespace low_pass_filter
{
template <uint16_t Number_of_iterations> struct arithmetic_mean
{
	bool operator()(uint16_t signal_value)
	{
		static uint16_t count{};
		if (count < Number_of_iterations)
		{
			mean_value += signal_value;
			++count;
			return false;
		}
		else
		{
			mean_value /= count;
			count = 0;
			return true;
		}
	};
	float operator()() const { return mean_value; }

private:
	float mean_value{};
};

} // namespace low_pass_filter
} // namespace wokwi

#endif // !LOW_PASS_FILTER_H