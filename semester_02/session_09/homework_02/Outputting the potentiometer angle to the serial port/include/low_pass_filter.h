// low_pass_filter.h

#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#include <stdint.h>
#include <Arduino.h>

namespace wokwi
{
namespace low_pass_filter
{
template <typename Type, uint8_t Number_of_iterations, uint8_t Polling_time_interval> struct arithmetic_mean
{
    using pointer_to_function_type = uint16_t (Type::*)(uint8_t) const;
    float operator()(const Type &object, pointer_to_function_type func, uint8_t value)
    {
        constexpr auto timeout = Polling_time_interval / Number_of_iterations;
        uint16_t sum{};
        for (uint8_t count{}; count < Number_of_iterations; ++count, delay(timeout))
            sum += (object.*func)(value);
        return sum / Number_of_iterations;
    };
};

} // namespace low_pass_filter
} // namespace wokwi

#endif // !LOW_PASS_FILTER_H