// Мигание в асинхронном режиме
// static const wokwi::triple_led test{{3, 4, 5}};
// test.blink(1000);

#include "rgb_color.h"
#include "triple_led.h"
#include "button.h"

void setup() {}

void loop()
{
  static wokwi::button button{2};
  button.tick();
  if (button.click())
  {
    static const wokwi::triple_led led{{3, 4, 5}};
    static auto color = wokwi::rgb_range::begin();

    if (color != wokwi::rgb_range::end())
      led.power_on(color++);
    else
    {
      led.power_off();
      color = wokwi::rgb_range::begin();
    }
  }
}