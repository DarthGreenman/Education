
#ifndef RGB_COLOR_H
#define RGB_COLOR_H

#include <stdint.h>

namespace wokwi
{
    enum class rgb_color : uint8_t
    {
        red,
        green,
        blue
    };
    wokwi::rgb_color &operator++(wokwi::rgb_color &color);
    wokwi::rgb_color operator++(wokwi::rgb_color &color, int);
    constexpr wokwi::rgb_color operator+(wokwi::rgb_color lhs, uint8_t rhs)
    {
        return static_cast<wokwi::rgb_color>(static_cast<uint8_t>(lhs) + rhs);
    }
    namespace rgb_range
    {
        constexpr wokwi::rgb_color begin() { return wokwi::rgb_color::red; };
        constexpr wokwi::rgb_color end() { return wokwi::rgb_color::blue + 1u; };
    }
}

#endif // !RGB_COLOR_H