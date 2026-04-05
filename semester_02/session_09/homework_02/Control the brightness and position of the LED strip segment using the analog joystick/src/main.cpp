
#include "hardware.h"
#include "joystick.h"
#include "led_strip.h"
#include "helper.h"
#include "low_pass_filter.h"
#include "task.h"

#include <Arduino.h>
#include <stddef.h>

#define _DEBUGGING_EVENTS
#define _DEBUGGING_ASYNCHRONOUS

using low_pass_filter = wokwi::low_pass_filter::arithmetic_mean<10>;
using pin_type_element = typename wokwi::joystick<low_pass_filter>::pin_type_element;
using bind = helper::pair<pin_type_element, uint8_t>;

wokwi::joystick<low_pass_filter>
    joystick{bind{pin_type_element::horz, A0}, bind{pin_type_element::vert, A1}, bind{pin_type_element::select, 2}};

wokwi::task<wokwi::joystick<low_pass_filter>> task_joystick{&wokwi::joystick<low_pass_filter>::update, &joystick, 10};

enum class brightness : uint8_t
{
    low = 0u,
    high = 250u
};
wokwi::light_emitting_diode_strip<11u, 10u, 9u, 6u, 5u, 3u>
    strip{wokwi::signal_type::analog, (static_cast<uint8_t>(brightness::high) - static_cast<uint8_t>(brightness::low)) / 2u};

void setup()
{
    Serial.begin(115200);
#ifndef DEBUGGING_EVENTS
    static auto signal_value = (static_cast<uint8_t>(brightness::high) - static_cast<uint8_t>(brightness::low)) / 2u;
    constexpr auto signal_value_step = (static_cast<uint8_t>(brightness::high) - static_cast<uint8_t>(brightness::low)) / 10u;
#endif
    const static auto pin_horz_number = joystick.get_pin_number(pin_type_element::horz);
    const static auto pin_vert_number = joystick.get_pin_number(pin_type_element::vert);

    joystick.right([]()
                   {
#ifdef DEBUGGING_EVENTS
                       Serial.println(analogRead(pin_horz_number));
#else
                       if (analogRead(pin_horz_number) == wokwi::analog_signal_low())
                           strip.shift_right();
#endif
                   });

    joystick.left([]()
                  {
#ifdef DEBUGGING_EVENTS
                      Serial.println(analogRead(pin_horz_number));
#else
                      if (analogRead(pin_horz_number) == wokwi::analog_signal_high())
                          strip.shift_left();
#endif
                  });

    joystick.up([]()
                {
#ifdef DEBUGGING_EVENTS
                    Serial.println(analogRead(pin_vert_number));
#else
                    if (analogRead(pin_vert_number) == wokwi::analog_signal_high())
                    {
                        strip.change_brightness(signal_value);
                        if (signal_value == static_cast<uint8_t>(brightness::high))
                            return;
                        signal_value += signal_value_step;
                    }
#endif
                });

    joystick.down([]()
                  {
#ifdef DEBUGGING_EVENTS
                      Serial.println(analogRead(pin_vert_number));
#else
                      if (analogRead(pin_vert_number) == wokwi::analog_signal_low())
                          {
                              strip.change_brightness(signal_value);
                              if (signal_value == static_cast<uint8_t>(brightness::low))
                                  return;
                              signal_value -= signal_value_step;
                          }
#endif
                  });

    joystick.select([]()
                    { strip.swap(); });
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
