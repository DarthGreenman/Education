
#include "rgb_color.h"

wokwi::RGB_COLOR& wokwi::operator++(wokwi::RGB_COLOR& color)
{
    color = static_cast<wokwi::RGB_COLOR>(static_cast<uint8_t>(color) + 1u);
    return color;
}
wokwi::RGB_COLOR wokwi::operator++(wokwi::RGB_COLOR& color, int)
{
    const auto prev = color;
    color = static_cast<wokwi::RGB_COLOR>(static_cast<uint8_t>(color) + 1u);
    return prev;
}