
#include "button.h"
#include <Arduino.h>

wokwi::button::button(uint8_t pin, unsigned long duration_bounce)
    : _pin{pin}, _duration_bounce{duration_bounce}
{
    pinMode(_pin, INPUT_PULLUP);
}

void wokwi::button::tick()
{
    bool state = !digitalRead(_pin);
    _click = false;

    if (_state != state && millis() - _timer >= _duration_bounce)
    {
        _state = state;
        _timer = millis();
        if (state)
            _click = true;
    }
}

bool wokwi::button::click() const { return _click; }
