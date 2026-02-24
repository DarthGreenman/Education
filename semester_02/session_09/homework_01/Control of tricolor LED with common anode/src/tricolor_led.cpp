
#include "tricolor_led.h"
#include <Arduino.h>

wokwi::tricolor_led::tricolor_led(const uint8_t (&pins)[3], common_pin com)
    : _voltage_on_com{com == common_pin::anode ? HIGH : LOW}
{
    for (auto color = wokwi::rgb_range::begin(); color != wokwi::rgb_range::end(); ++color)
    {
        const auto color_index = static_cast<uint8_t>(color);
        static size_t pin_index{};
        _channel[color_index] = pins[pin_index++];
        construct_connection_diagram(color_index, com);
    }
}

void wokwi::tricolor_led::power_on(wokwi::rgb_color color) const
{
    const auto color_index = static_cast<uint8_t>(color);
    digitalWrite(_channel[color_index], !_voltage_on_com);
}
void wokwi::tricolor_led::power_on() const
{
    for (auto color = wokwi::rgb_range::begin(); color != wokwi::rgb_range::end(); ++color)
        power_on(color);
}
void wokwi::tricolor_led::power_off(wokwi::rgb_color color) const
{
    const auto color_index = static_cast<uint8_t>(color);
    digitalWrite(_channel[color_index], _voltage_on_com);
}
void wokwi::tricolor_led::power_off() const
{
    for (auto color = wokwi::rgb_range::begin(); color != wokwi::rgb_range::end(); ++color)
        power_off(color);
}

void wokwi::tricolor_led::construct_connection_diagram(uint8_t pin, common_pin com) const
{
    pinMode(_channel[pin], OUTPUT);
    if (com == common_pin::anode)
        digitalWrite(_channel[pin], _voltage_on_com);
}
