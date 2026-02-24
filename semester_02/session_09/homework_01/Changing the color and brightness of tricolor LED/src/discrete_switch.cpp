
#include "discrete_switch.h"

wokwi::discrete_switch::discrete_switch(uint8_t increase_button, uint8_t decrease_button, int discreteness,
                                        unsigned long duration_bounce)
    : _increase_button{increase_button, duration_bounce},
      _decrease_button{decrease_button, duration_bounce},
      _discreteness{discreteness}
{
}

void wokwi::discrete_switch::tick()
{
    _increase_button.tick();
    _decrease_button.tick();
}

bool wokwi::discrete_switch::inc()
{
    if (_increase_button.click() && _counter < _discreteness)
    {
        ++_counter;
        return true;
    }
    return false;
}

bool wokwi::discrete_switch::dec()
{
    if (_decrease_button.click() && _counter)
    {
        --_counter;
        return true;
    }

    return false;
}
