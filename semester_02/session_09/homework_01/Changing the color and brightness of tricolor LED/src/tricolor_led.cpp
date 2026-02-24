
#include "tricolor_led.h"
#include <Arduino.h>

wokwi::tricolor_led::tricolor_led(const uint8_t (&pins)[3], common_pin com)
    : _voltage_on_com{com == common_pin::anode ? HIGH : LOW}
{
    for (auto color = wokwi::rgb_range::begin(); color != wokwi::rgb_range::end(); ++color)
    {
        static size_t pin_index{};
        const auto color_index =  wokwi::helper::color_index(color);
        _channel[color_index] = pins[pin_index++];
        _brightness[color_index] = (com == common_pin::anode ? _dimming_depth : 0u);
        construct_connection_diagram(color_index, com);
    }
}

void wokwi::tricolor_led::power_on(wokwi::rgb_color color) const
{
    digitalWrite(_channel[wokwi::helper::color_index(color)], !_voltage_on_com);
}
void wokwi::tricolor_led::power_on() const
{
    for (auto color = wokwi::rgb_range::begin(); color != wokwi::rgb_range::end(); ++color)
        power_on(color);
}
void wokwi::tricolor_led::power_off(wokwi::rgb_color color) const
{
    digitalWrite(_channel[wokwi::helper::color_index(color)], _voltage_on_com);
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