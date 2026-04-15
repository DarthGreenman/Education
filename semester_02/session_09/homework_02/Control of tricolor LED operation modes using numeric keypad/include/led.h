// led.h

#ifndef LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT
#define LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "fox.h"
#include "hardware.h"

#include <stdint.h>

namespace uno {
template <typename Ts>
class light_emitting_diode : public uno::electronic_component<Ts, uno::signal_direction::output> {
public:
    using basic = uno::electronic_component<Ts, uno::signal_direction::output>;

    constexpr light_emitting_diode() = default;
    /**
     * @brief Construct a new light emitting diode object
     *
     * @param pin_number
     */
    explicit light_emitting_diode(uint8_t pin_number)
        : basic(pin_number)
    {
    }

    light_emitting_diode(const light_emitting_diode&) = default;
    light_emitting_diode(light_emitting_diode&&) = default;
    ~light_emitting_diode() = default;

    light_emitting_diode& operator=(const light_emitting_diode&) = default;
    light_emitting_diode& operator=(light_emitting_diode&&) = default;

    /**
     * @brief
     *
     */
    void blink()
    {
        static_assert(fox::is_same<Ts, uno::signal_analog>::value,
            "This function is for blink of the LED with an digital signal.");

        !basic::read() ? basic::write(uno::signal_digital::high)
                       : basic::write(uno::signal_digital::low);
    }
    /**
     * @brief
     *
     * @param signal_value
     */
    void change_brightness(uint8_t signal_value)
    {
        static_assert(fox::is_same<Ts, uno::signal_analog>::value,
            "This function is for changing the brightness of the LED with an analog signal.");
        basic::write(signal_value);
    }
};
}; // namespace uno

#endif // !LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT