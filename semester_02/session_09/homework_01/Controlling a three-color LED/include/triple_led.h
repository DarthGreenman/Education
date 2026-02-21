
#ifndef TRIPLE_LED_H
#define TRIPLE_LED_H

#include <stdint.h>
#include <stddef.h>
#include <Arduino.h>

#include "rgb_color.h"
#include "single_led.h"

namespace wokwi
{
    class triple_led
    {
    public:
        explicit triple_led(const uint8_t (&pins)[3]);
        triple_led(const triple_led &) = delete;
        triple_led(triple_led &&) = delete;
        ~triple_led() = default;

        triple_led &operator=(const triple_led &) = delete;
        triple_led &operator=(triple_led &&) = delete;

        void blink() const;
        template <size_t N>
        void blink(const wokwi::rgb_color (&mix)[N]) const;

        void power_on(wokwi::rgb_color color) const;
        void power_on() const;
        void power_off(wokwi::rgb_color color) const;
        void power_off() const;

    private:
        const uint8_t get_pin(wokwi::rgb_color color) const;
        wokwi::single_led _led[3];
    };

    template <size_t N>
    inline void triple_led::blink(const wokwi::rgb_color (&mix)[N]) const
    {
        static bool mode{LOW};
        for (auto color = 0ull; color < N; ++color)
        {
            const auto pin = get_pin(mix[color]);
            (!mode) ? _led[pin].power_on() : _led[pin].power_off();
        }
        mode = !mode;
    }
}

#endif // !TRIPLE_LED_H