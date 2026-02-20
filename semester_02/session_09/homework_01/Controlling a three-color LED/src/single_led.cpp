
#include "single_led.h"
#include <Arduino.h>

wokwi::single_led::single_led(uint8_t pin) : _pin{pin} { pinMode(_pin, OUTPUT); }

void wokwi::single_led::blink(unsigned long timeout) const
{
    for (auto mode = get_number_of_modes(); mode-- > 0;)
        mode == 0 ? power_off() : power_on();
    delay(timeout);
}
void wokwi::single_led::power_on() const { digitalWrite(_pin, HIGH); }
void wokwi::single_led::power_off() const { digitalWrite(_pin, LOW); }
