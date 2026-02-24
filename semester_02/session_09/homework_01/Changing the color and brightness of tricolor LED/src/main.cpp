
/*
 Для расчета резистора, ограничивающего ток светодиода,
 используется формула: R = (V - Vl) / Il
 Ток питающий светодиоды - 20 мА.

 Анод "красного" светодиода подключен к 9 пину, катод к 5V.
 R = (5,00 - 1,83) / 0,020 = 158,50 Ом
 Анод "зеленого" светодиода подключен к 10 пину, катод к 5V.
 R = (5,00 - 2,95) / 0,020 = 102,50 Ом
 Анод "синего" светодиода подключен к 11 пину, катод к 5V.
 R = (5,00 - 3,50) / 0,020 = 95,5 Ом
*/

#include <Arduino.h>
#include "button.h"
#include "tricolor_led.h"
#include "discrete_switch.h"

constexpr uint8_t step{10};
using discrete_switch = wokwi::discrete_switch;
discrete_switch control[]{
    discrete_switch{2, 3, step},  /* red */
    discrete_switch{4, 5, step},  /* green */
    discrete_switch{6, 7, step}}; /* blue */

auto tick = [&]()
{
  static constexpr auto number_of_control_elements = sizeof(control) / sizeof(control[0]);
  for (size_t elem{}; elem < number_of_control_elements; ++elem)
    control[elem].tick();
};

wokwi::tricolor_led led{{9, 10, 11}, wokwi::tricolor_led::common_pin::anode};
auto inc = [&](wokwi::rgb_color color)
{ led.brightness_control(color, step, wokwi::inc); };
auto dec = [&](wokwi::rgb_color color)
{ led.brightness_control(color, step, wokwi::dec); };

void setup() {}
void loop()
{
  tick();
  
  if (control[0].inc())
    inc(wokwi::rgb_color::red);
  if (control[0].dec())
    dec(wokwi::rgb_color::red);

  if (control[1].inc())
    inc(wokwi::rgb_color::green);
  if (control[1].dec())
    dec(wokwi::rgb_color::green);

  if (control[2].inc())
    inc(wokwi::rgb_color::blue);
  if (control[2].dec())
    dec(wokwi::rgb_color::blue);
}
