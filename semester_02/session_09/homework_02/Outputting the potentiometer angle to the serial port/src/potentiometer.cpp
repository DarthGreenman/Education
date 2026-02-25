// potentiometer.cpp

#include "potentiometer.h"
#include "time.h"

wokwi::potentiometer::potentiometer(uint8_t pin, int low, int high, uint32_t data_polling_period)
    : _pin{pin}, _turning_range{low, high}, _data_polling_period{data_polling_period}
{
}

void wokwi::potentiometer::tick()
{
    _turned = false;
    const auto signal_value = analogRead(_pin);

    auto turning = [this, signal_value]()
    {
        _signal_value = signal_value;
        if (_signal_value < _turning_range.low || _signal_value > _turning_range.high)
            _turned = true;
    };

    if (signal_value != _signal_value)
        wokwi::tick(_data_polling_period, turning);
}

float wokwi::potentiometer::angle() const
{
    const auto signal_value = analogRead(_pin);
    const auto signal_range = wokwi::analog_signal_range.high - wokwi::analog_signal_range.low;
    const auto turning_range = _turning_range.high - _turning_range.low;
    const auto signal_value_step = turning_range / signal_range;

    return 0.0f;
}
