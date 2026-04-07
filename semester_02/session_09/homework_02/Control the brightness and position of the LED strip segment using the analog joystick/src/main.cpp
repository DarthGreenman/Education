
#include "hardware.h"
#include "joystick.h"
#include "led_strip.h"
#include "fox.h"
#include "low_pass_filter.h"
#include "task.h"
#include "types_used.h"

#include <Arduino.h>
#include <stddef.h>

#define _DEBUGGING_EVENTS
#define _DEBUGGING_ASYNCHRONOUS

using pin_type_element = typename wokwi::joystick::pin_type_element;
wokwi::joystick joystick{
    fox::make_pair(pin_type_element::horz, A0),
    fox::make_pair(pin_type_element::vert, A1),
    fox::make_pair(pin_type_element::select, 2)};

wokwi::task<wokwi::joystick> task_joystick{&wokwi::joystick::update, &joystick, 150};

enum brightness
{
    low = 0,
    high = 250
};

wokwi::light_emitting_diode_strip<wokwi::signal_analog, 11u, 10u, 9u, 6u, 5u, 3u> led_strip{125u};

void setup()
{
    Serial.begin(115200);

#ifndef DEBUGGING_EVENTS
    static auto signal_value_brightness = (static_cast<uint8_t>(brightness::high) - static_cast<uint8_t>(brightness::low)) / 2u;
    constexpr auto signal_value_step = (static_cast<uint8_t>(brightness::high) - static_cast<uint8_t>(brightness::low)) / 10u;
#endif
    joystick.right([]()
                   {
#ifdef DEBUGGING_EVENTS
                       Serial.println(joystick.get_signal_axes().first);
#else
                       if (joystick.get_signal_axes().first == wokwi::signal_analog::low)
                           led_strip.shift_right();
#endif
                   });

    joystick.left([]()
                  {
#ifdef DEBUGGING_EVENTS
                      Serial.println(joystick.get_signal_axes().first);
#else
                      if (joystick.get_signal_axes().first == wokwi::signal_analog::high)
                          led_strip.shift_left();
#endif
                  });

    joystick.up([]()
                {
#ifdef DEBUGGING_EVENTS
                    Serial.println(joystick.get_signal_axes().second);
#else
                    if (joystick.get_signal_axes().second == wokwi::signal_analog::high)
                    {
                        led_strip.change_brightness(signal_value_brightness);
                        if (signal_value_brightness == static_cast<uint8_t>(brightness::high))
                            return;
                        signal_value_brightness += signal_value_step;
                    }
#endif
                });

    joystick.down([]()
                  {
#ifdef DEBUGGING_EVENTS
                      Serial.println(joystick.get_signal_axes().second);
#else
                      if (joystick.get_signal_axes().second == wokwi::signal_analog::low)
                      {
                          led_strip.change_brightness(signal_value_brightness);
                          if (signal_value_brightness == static_cast<uint8_t>(brightness::low))
                              return;
                          signal_value_brightness -= signal_value_step;
                      }
#endif
                  });

    joystick.select([]()
                    { led_strip.swap(); });
}

void loop()
{
    task_joystick.exec();
#ifdef DEBUGGING_ASYNCHRONOUS
    static uint32_t uptime{};
    if (millis() - uptime >= 1000ul)
        Serial.println(uptime = millis());
#endif
}
