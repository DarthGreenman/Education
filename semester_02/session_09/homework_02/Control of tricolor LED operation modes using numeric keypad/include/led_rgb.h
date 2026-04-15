// led_rgb.h

#ifndef LIGHT_EMITTING_DIODE_RGB_H
#define LIGHT_EMITTING_DIODE_RGB_H

#include "fox.h"
#include "hardware.h"
#include "types_used.h"

#include <stdint.h>

namespace uno {
template <typename Ts>
class light_emitting_diode_rgb {
    static_assert(fox::is_same<Ts, uno::signal_analog>::value,
        "This class is designed to work with an analog signal.");

public:
    using light_emitting_diode = uno::electronic_component<Ts, uno::signal_direction::output>;
    using channels = uno::color::rgb<uno::electronic_component<Ts, uno::signal_direction::output>>;
    using pins = uno::color::rgb<uint8_t>;

    /**
     * @brief
     *
     */
    enum class common_pin : uint8_t {
        anode,
        cathode
    };

    constexpr light_emitting_diode_rgb() = default;
    /**
     * @brief Construct a new light emitting diode color object
     *
     * @param pin_numbers
     * @param com
     */
    explicit light_emitting_diode_rgb(const pins& pin_numbers, common_pin com)
        : _channels {
            light_emitting_diode { pin_numbers.red },
            light_emitting_diode { pin_numbers.green },
            light_emitting_diode { pin_numbers.blue }
        }
        , _com { com }
    {
        using namespace uno;
        const auto channel = color::unpack_rgb(
            com == common_pin::anode && !fox::is_same<Ts, signal_digital>::value
                ? color::red | color::green | color::blue
                : com == common_pin::anode && fox::is_same<Ts, signal_digital>::value
                ? signal_digital::high
                : 0u);
        write(channel.red, channel.green, channel.blue);
    }
    light_emitting_diode_rgb(const light_emitting_diode_rgb&) = default;
    light_emitting_diode_rgb(light_emitting_diode_rgb&&) = default;
    ~light_emitting_diode_rgb() = default;

    light_emitting_diode_rgb& operator=(const light_emitting_diode_rgb&) = default;
    light_emitting_diode_rgb& operator=(light_emitting_diode_rgb&&) = default;

    /**
     * @brief
     *
     */
    void blink(uint32_t value)
    {
        static bool is_blink { };
        color_change((is_blink = !is_blink) ? value : 0u);
    }

    /**
     * @brief
     *
     * @param value
     */
    void color_change(uint32_t value)
    {
        const auto channel
            = uno::color::unpack_rgb(value = _com == common_pin::anode
                    ? ~value
                    : value);
        write(channel.red, channel.green, channel.blue);
        
    }

private:
    /**
     * @brief
     *
     * @param red_value
     * @param green_value
     * @param blue_value
     */
    void write(uint8_t red_value, uint8_t green_value, uint8_t blue_value)
    {
        _channels.red.write(red_value);
        _channels.green.write(green_value);
        _channels.blue.write(blue_value);
    }

    channels _channels { };
    common_pin _com { };
};
} // namespace uno

#endif // !LIGHT_EMITTING_DIODE_RGB_H
