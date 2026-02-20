// diagram.json

#include "rgb_color.h"
#include "triple_led.h"
#include "button.h"

void setup() {}

void loop()
{
  static const wokwi::button button{2};
  static auto prev_state = LOW;
  const auto curr_state = button.get_state(prev_state);

  if (curr_state == LOW)
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
    prev_state = curr_state;
    delay(button.get_response_time());
  }
}