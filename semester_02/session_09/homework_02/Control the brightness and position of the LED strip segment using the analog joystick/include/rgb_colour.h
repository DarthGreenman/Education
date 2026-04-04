// rgb_colour.h

#ifndef RGB_COLOUR_H
#define RGB_COLOUR_H

#include <stdint.h>

namespace wokwi
{
    enum class rgb_colour : uint8_t
    {
        red,
        green,
        blue
    };
    wokwi::rgb_colour &operator++(wokwi::rgb_colour &colour)
    {
        colour = static_cast<wokwi::rgb_colour>(static_cast<uint8_t>(colour) + 1u);
        return colour;
    }
    wokwi::rgb_colour operator++(wokwi::rgb_colour &colour, int)
    {
        const auto prev = colour;
        colour = static_cast<wokwi::rgb_colour>(static_cast<uint8_t>(colour) + 1u);
        return prev;
    }
    constexpr wokwi::rgb_colour operator+(wokwi::rgb_colour lhs, uint8_t rhs)
    {
        return static_cast<wokwi::rgb_colour>(static_cast<uint8_t>(lhs) + rhs);
    }

    namespace rgb_range
    {
        constexpr wokwi::rgb_colour begin()
        {
            return wokwi::rgb_colour::red;
        };
        constexpr wokwi::rgb_colour end()
        {
            return wokwi::rgb_colour::blue + 1u;
        };
    } // namespace rgb_range
} // namespace wokwi

#endif // !RGB_COLOUR_H