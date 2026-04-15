// events.h

#ifndef EVENTS_H_FOR_THE_ARDUINO_UNO_PROJECT
#define EVENTS_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <stdint.h>

namespace uno {
enum class button_events : uint8_t {
    release,
    pressed,
    begin = release,
    end = pressed + 1u
};

enum class variable_resistor_events : uint8_t {
    decreased_resistance,
    increased_resistance,
    begin = decreased_resistance,
    end = increased_resistance + 1u
};

enum class joystick_events : uint8_t {
    left, // Увеличение сигнала - уменьшение сопротивления
    right, // Уменьшение сигнала - увеличение сопротивления
    up, // Увеличение сигнала - уменьшение сопротивления
    down, // Уменьшение сигнала - увеличение сопротивления
    selection, // ...
    begin = left,
    end = selection + 1u
};

enum class key_events : uint8_t {
    up,
    down,
    begin = up,
    end = down + 1u
};

template <typename E>
static E& operator++(E& event)
{
    event = static_cast<E>(static_cast<uint8_t>(event) + 1u);
    return event;
}
} // namespace uno

#endif // !EVENTS_H_FOR_THE_ARDUINO_UNO_PROJECT