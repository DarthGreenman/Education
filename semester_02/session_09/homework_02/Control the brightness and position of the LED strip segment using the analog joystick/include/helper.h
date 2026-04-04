// helper.h

#ifndef HELPER_H
#define HELPER_H

#include <stddef.h>
#include <stdint.h>

namespace helper
{
    struct expand_parameter_package
    {
        template <typename... Types>
        expand_parameter_package(Types...) {}
    };
    template <typename ValueTypeFirst, typename ValueTypeSecond>
    struct pair
    {
        ValueTypeFirst first;
        ValueTypeSecond second;
    };

    template <typename ValueType, size_t N>
    size_t find(const ValueType (&elems)[N], ValueType elem)
    {
        for (size_t n{}; n < N; ++n)
            if (elems[n] == elem)
                return n;
        return N;
    }
    template <typename ValueType, typename Comparison_condition, size_t N>
    size_t find_if(const ValueType (&elems)[N], Comparison_condition condition)
    {
        for (size_t n{}; n < N; ++n)
            if (condition(n))
                return n;
        return N;
    }
    template <typename ValueType>
    float map(ValueType x, ValueType in_min, ValueType in_max, ValueType out_min, ValueType out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
} // namespace helper

#endif // HELPER_H