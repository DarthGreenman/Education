
#include "rgb_color.h"

wokwi::rgb_color &wokwi::operator++(wokwi::rgb_color &color)
{
    color = static_cast<wokwi::rgb_color>(static_cast<uint8_t>(color) + 1u);
    return color;
}

wokwi::rgb_color wokwi::operator++(wokwi::rgb_color &color, int)
{
    const auto prev = color;
    color = static_cast<wokwi::rgb_color>(static_cast<uint8_t>(color) + 1u);
    return prev;
}