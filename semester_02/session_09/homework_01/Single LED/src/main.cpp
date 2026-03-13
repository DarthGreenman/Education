/*
 Для расчета резистора, ограничивающего ток светодиода,
 используется формула: R = (V - Vl) / Il
 Ток питающий светодиоды - 20 мА.

 Анод "красного" светодиода подключен к 13 пину, катод к GND.
 R = (5,00 - 1,83) / 0,020 = 158,50 Ом

 Анод "белого" светодиода подключен к 12 пину, катод к GND.
 R = (5,00 - 3,50) / 0,020 = 75 Ом
*/

#include <Arduino.h>
#include "single_led.h"

const wokwi::single_led led_red{13};
const wokwi::single_led led_white{12};

void setup() {}

void loop()
{
  led_red.power_on();
  led_white.power_on();
  delay(500);

  led_red.power_off();
  led_white.power_off();
  delay(500);
}
