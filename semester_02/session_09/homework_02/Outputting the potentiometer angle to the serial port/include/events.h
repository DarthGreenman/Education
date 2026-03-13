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
    click,
    hold,
    begin = release,
    end = hold + 1u
}; // enum class button_events

enum class potentiometer_events : uint8_t
{
    turn,
    begin = turn,
    end = turn + 1u
}; // enum class POTENTIOMETER_EVENTS

template <typename Events> static Events &operator++(Events &event)
{
    event = static_cast<Events>(static_cast<uint8_t>(event) + 1u);
    return event;
}
} // namespace wokwi

#endif // !EVENTS_H