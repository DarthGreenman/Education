
#include "potentiometer.h"
#include <Arduino.h>

wokwi::potentiometer pot{A0, -134, 135, 100};

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  pot.tick();
  if (pot.turned())
    Serial.println(pot.angle());
}