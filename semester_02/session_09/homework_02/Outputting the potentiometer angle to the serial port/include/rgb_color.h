// RGB_COLOR.h

#ifndef RGB_COLOR_H
#define RGB_COLOR_H

#include <stdint.h>

namespace wokwi
{
enum class RGB_COLOR : uint8_t
{
    RED,
    GREEN,
    BLUE
}; // enum class RGB_COLOR : uint8_t

wokwi::RGB_COLOR &operator++(wokwi::RGB_COLOR &color);
wokwi::RGB_COLOR operator++(wokwi::RGB_COLOR &color, int);
constexpr wokwi::RGB_COLOR operator+(wokwi::RGB_COLOR lhs, uint8_t rhs)
{
    return static_cast<wokwi::RGB_COLOR>(static_cast<uint8_t>(lhs) + rhs);
}

namespace rgb_range
{
constexpr wokwi::RGB_COLOR begin()
{
    return wokwi::RGB_COLOR::RED;
};
constexpr wokwi::RGB_COLOR end()
{
    return wokwi::RGB_COLOR::BLUE + 1u;
};
} // namespace rgb_range
} // namespace wokwi

#endif // !RGB_COLOR_H