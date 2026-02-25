
#include "button.h"
#include "time.h"
#include <Arduino.h>

wokwi::button::button(uint8_t pin, uint32_t duration_bounce)
    : _duration_bounce{duration_bounce}, _pin{pin}
{
    pinMode(_pin, INPUT_PULLUP);
}

void wokwi::button::tick()
{
    _click = false;
    bool state{!digitalRead(_pin)};
    auto curr_state = [&, state]()
    {
        _state = state;
        if (state)
            _click = true;
    };

    if (_state != state)
        wokwi::tick(_duration_bounce, curr_state);
}

bool wokwi::button::click() const { return _click; }