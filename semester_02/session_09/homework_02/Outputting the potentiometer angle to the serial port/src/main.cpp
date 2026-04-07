
#include "hardware.h"
#include "led.h"
#include "variable_resistor.h"
#include "task.h"
#include "types_used.h"

#include <Arduino.h>
#include <stddef.h>

#define _DEBUG

constexpr uint32_t polling_time_interval = 1;
wokwi::variable_resistor var_reg{A0};
wokwi::task<wokwi::variable_resistor> task_pot{&wokwi::variable_resistor::update, &var_reg, 100u};

using led = wokwi::light_emitting_diode<wokwi::signal_digital>;
wokwi::task<led> task_led[]{wokwi::task<led>{&led::blink, led{9}, 1000u, 2000u},
                            wokwi::task<led>{&led::blink, led{13}, 1000u}};
constexpr auto number_of_tasks = sizeof(task_led) / sizeof(task_led[0]);

void setup()
{
    Serial.begin(115200);
    var_reg.decreased_resistance([]()
                             {
        const auto value = var_reg.get_signal(-135.0f, 134.0f);
        Serial.println(value); });
    var_reg.increased_resistance([]()
                             {
        const auto value = var_reg.get_signal(-135.0f, 134.0f);
        Serial.println(value); });
}

void loop()
{
    task_pot.exec();
    for (size_t i{}; i < number_of_tasks; ++i)
        task_led[i].exec();

#ifdef DEBUG
    static uint32_t uptime{};
    if (millis() - uptime >= 1000ul)
    {
        Serial.println(uptime = millis());
        Serial.println(var_reg.get_signal());
    }
#endif
}