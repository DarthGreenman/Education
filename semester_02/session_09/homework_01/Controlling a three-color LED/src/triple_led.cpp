
#include "triple_led.h"

wokwi::triple_led::triple_led(const uint8_t (&pins)[3])
{
    for (auto color = wokwi::rgb_range::begin(); color != wokwi::rgb_range::end(); ++color)
    {
        const auto pin = get_pin(color);
        _led[pin] = wokwi::single_led{pins[pin]};
    }
}

void wokwi::triple_led::blink() const
{
    blink({wokwi::rgb_color::red, wokwi::rgb_color::green, wokwi::rgb_color::blue});
}

void wokwi::triple_led::power_on(wokwi::rgb_color color) const { _led[get_pin(color)].power_on(); }
void wokwi::triple_led::power_on() const
{
    for (auto color = wokwi::rgb_range::begin(); color != wokwi::rgb_range::end(); ++color)
        power_on(color);
}
void wokwi::triple_led::power_off(wokwi::rgb_color color) const { _led[get_pin(color)].power_off(); }
void wokwi::triple_led::power_off() const
{
    for (auto color = wokwi::rgb_range::begin(); color != wokwi::rgb_range::end(); ++color)
        power_off(color);
}

const uint8_t wokwi::triple_led::get_pin(wokwi::rgb_color color) const { return static_cast<uint8_t>(color); }