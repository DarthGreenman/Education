
#include "rgb_colour.h"

wokwi::rgb_colour &wokwi::operator++(wokwi::rgb_colour &colour)
{
    colour = static_cast<wokwi::rgb_colour>(static_cast<uint8_t>(colour) + 1u);
    return colour;
}
wokwi::rgb_colour wokwi::operator++(wokwi::rgb_colour &colour, int)
{
    const auto prev = colour;
    colour = static_cast<wokwi::rgb_colour>(static_cast<uint8_t>(colour) + 1u);
    return prev;
}