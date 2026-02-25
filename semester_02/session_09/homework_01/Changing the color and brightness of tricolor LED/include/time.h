
#ifndef TIME_H
#define TIME_H

#include <stdint.h>
#include <Arduino.h>

namespace wokwi
{
    template <typename Action>
    void tick(uint32_t timeout, Action action)
    {
        static uint32_t uptime{};
        if (millis() - uptime >= timeout)
        {
            uptime = millis();
            action();
        }
    }

} // namespace wokwi

#endif