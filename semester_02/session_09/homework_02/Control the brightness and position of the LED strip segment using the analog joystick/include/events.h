// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

namespace wokwi
{
    enum class button_events : uint8_t
    {
        release,
        pressed,
        begin = release,
        end = pressed + 1u
    };
    enum class variable_resistor_events : uint8_t
    {
        decreased_resistance,
        increased_resistance,
        begin = decreased_resistance,
        end = increased_resistance + 1u
    };
    enum class joystick_events : uint8_t
    {
        left,      // Увеличение сигнала - уменьшение сопротивления
        right,     // Уменьшение сигнала - увеличение сопротивления
        up,        // Увеличение сигнала - уменьшение сопротивления
        down,      // Уменьшение сигнала - увеличение сопротивления
        selection, // ...
        begin = left,
        end = selection + 1u
    }; // enum class joystick_events

    template <typename Events>
    static Events &operator++(Events &event)
    {
        event = static_cast<Events>(static_cast<uint8_t>(event) + 1u);
        return event;
    }
} // namespace wokwi

#endif // !EVENTS_H