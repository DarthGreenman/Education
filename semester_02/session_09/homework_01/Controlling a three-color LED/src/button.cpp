
#include "button.h"
#include <Arduino.h>

wokwi::button::button(uint8_t pin, unsigned long timeout) : _pin{pin}, _timeout{timeout}
{
    pinMode(_pin, INPUT_PULLUP);
}

int wokwi::button::get_state(int state) const
{
    auto curr_state = digitalRead(_pin);
    if (state != curr_state)
    {
        delay(_timeout);
        curr_state = digitalRead(_pin);
    }
    return curr_state;
}

unsigned long wokwi::button::get_response_time() const { return _timeout * 2; }
