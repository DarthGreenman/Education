// low_pass_filter.h

#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#include "helper.h"

#include <stdint.h>

namespace wokwi
{
    namespace low_pass_filter
    {
        template <uint8_t NumberOfIterations>
        struct arithmetic_mean
        {
            uint16_t operator()(uint16_t signal_value)
            {
                static uint32_t sum{};
                sum += signal_value;
                if (++_count == NumberOfIterations)
                {
                    _value = sum / NumberOfIterations;
                    _count = 0u;
                    sum = 0ul;
                }
                return _value;
            };

        private:
            uint16_t _value{};
            uint8_t _count{};
        };
    } // namespace low_pass_filter
} // namespace wokwi

#endif // LOW_PASS_FILTER_H