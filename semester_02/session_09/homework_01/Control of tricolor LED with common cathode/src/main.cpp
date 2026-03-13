
#include "rgb_color.h"
#include "tricolor_led.h"
#include "button.h"

void setup() {}

void loop()
{
  static wokwi::button button{2};
  button.tick();
  if (button.click())
  {
    static const wokwi::tricolor_led led{{3, 4, 5}, wokwi::tricolor_led::common_pin::cathode};
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