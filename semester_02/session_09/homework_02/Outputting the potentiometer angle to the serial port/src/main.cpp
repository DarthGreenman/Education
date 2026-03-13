
#include "hardware.h"
#include "led.h"
#include "low_pass_filter.h"
#include "potentiometer.h"
#include "task.h"

#include <Arduino.h>
#include <stddef.h>

using signal_type = wokwi::signal_type;
using signal_direction = wokwi::signal_direction;
constexpr auto polling_time_interval = 32u;

using low_pass_filter =
    wokwi::low_pass_filter::arithmetic_mean<wokwi::electronic_component<1u>, 8, polling_time_interval>;
using potentiometer = wokwi::potentiometer<low_pass_filter>;

potentiometer pot{{{A0, signal_type::analog, signal_direction::input}}, -135, 135, low_pass_filter{}};
wokwi::task<potentiometer> task_pot{&potentiometer::update, &pot, polling_time_interval};

using led = wokwi::light_emitting_diode<1u>;
wokwi::task<led> task_led[]{
    wokwi::task<led>{&led::blink, led{{{9, signal_type::digital, signal_direction::output}}}, 1000, 2000},
    wokwi::task<led>{&led::blink, led{{{13, signal_type::digital, signal_direction::output}}}, 1000}};
constexpr auto number_of_tasks = sizeof(task_led) / sizeof(task_led[0]);

void setup()
{
    Serial.begin(115200);
    pot.set_turning_callback([]() {
        const auto value = pot.get_the_output_signal();
        Serial.println(value);
    });
}

void loop()
{
    task_pot.exec();
    for (size_t i{}; i < number_of_tasks; ++i)
        task_led[i].exec();
}