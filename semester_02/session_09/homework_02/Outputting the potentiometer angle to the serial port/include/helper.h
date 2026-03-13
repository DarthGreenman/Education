// helper.h

#ifndef HELPER_H
#define HELPER_H

#include <stddef.h>
#include <stdint.h>

namespace helper
{
template <typename Type, size_t N> size_t find(const Type (&elems)[N], Type elem)
{
    for (size_t n{}; n < N; ++n)
        if (elems[n] == elem)
            return n;
    return N;
}

template <typename Type_first, typename Type_second> struct couple
{
    Type_first first;
    Type_second second;
}; // template<typename Type> struct couple

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

} // namespace helper

#endif // HELPER_H