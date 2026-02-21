
#ifndef SINGLE_LED_H
#define SINGLE_LED_H

#include <stdint.h>
#include <stddef.h>

namespace wokwi
{
    class single_led
    {
    public:
        constexpr single_led() = default;
        explicit single_led(uint8_t pin);
        ~single_led() = default;
        
        void blink() const;
        void power_on() const;
        void power_off() const;

    private:
        uint8_t _pin{};
    };
}

#endif // !SINGLE_LED_H