
#include "single_led.h"
#include <Arduino.h>

wokwi::single_led::single_led(uint8_t pin) : _pin{pin} { pinMode(_pin, OUTPUT); }

void wokwi::single_led::blink(unsigned long timeout) const
{
    static unsigned long time{};
    if (millis() - time >= timeout)
    {
        static bool mode{LOW};
        !(mode) ? power_on() : power_off();
        mode = !mode;
        time = millis();
    }
}
void wokwi::single_led::power_on() const { digitalWrite(_pin, HIGH); }
void wokwi::single_led::power_off() const { digitalWrite(_pin, LOW); }
