// led_strip.h

#ifndef LIGHT_EMITTING_DIODE_STRIP_H
#define LIGHT_EMITTING_DIODE_STRIP_H

#include "hardware.h"
#include "helper.h"
#include "led.h"

#include <assert.h>

namespace wokwi
{
    template <uint8_t... PinNumbers>
    class light_emitting_diode_strip
    {
    public:
        static constexpr auto number_of_diode = sizeof...(PinNumbers);

        constexpr light_emitting_diode_strip() = default;
        explicit light_emitting_diode_strip(wokwi::signal_type signal_type, uint16_t signal_value);
        light_emitting_diode_strip(const light_emitting_diode_strip &) = delete;
        light_emitting_diode_strip(light_emitting_diode_strip &&) = delete;
        ~light_emitting_diode_strip() = default;

        light_emitting_diode_strip &operator=(const light_emitting_diode_strip &) = delete;
        light_emitting_diode_strip &operator=(light_emitting_diode_strip &&) = delete;

        void shift_right();
        void shift_left();
        void swap();
        void change_brightness(uint16_t signal_value);

    private:
        void construct(uint8_t pin_number, wokwi::signal_type signal_type, uint16_t signal_value);
        uint8_t get_current_position() const { return _current_position; }
        uint16_t get_signal() const { return _signal_value; }
        template <typename Function>
        void shift(uint8_t limit, Function action);

        wokwi::light_emitting_diode _led_strip[number_of_diode]{};
        constexpr static helper::pair<uint8_t, uint8_t> _range{static_cast<uint8_t>(-1), number_of_diode};
        uint16_t _signal_value{};
        uint8_t _current_position{};
    };
    /**
     * @brief
     * @param signal_type
     * @param signal_value
     */
    template <uint8_t... PinNumbers>
    inline light_emitting_diode_strip<PinNumbers...>::light_emitting_diode_strip(
        wokwi::signal_type signal_type, uint16_t signal_value) : _signal_value{signal_value}, _current_position{}
    {
        helper::expand_parameter_package{(construct(PinNumbers, signal_type, signal_value), 1)...};
        _current_position = _range.first;
    }
    /**
     * @brief
     */
    template <uint8_t... PinNumbers>
    inline void light_emitting_diode_strip<PinNumbers...>::shift_right()
    {
        shift(_range.second, [this]()
              { ++_current_position; });
    }
    template <uint8_t... PinNumbers>
    inline void light_emitting_diode_strip<PinNumbers...>::shift_left()
    {
        shift(_range.first, [this]()
              { --_current_position; });
    }
    template <uint8_t... PinNumbers>
    inline void light_emitting_diode_strip<PinNumbers...>::swap()
    {
        if (get_signal() && get_current_position() == 0)
            shift(_range.second, [this]()
                  { _current_position += _range.second - 1u; });
        else if (get_signal() && get_current_position() == _range.second - 1u)
            shift(_range.first, [this]()
                  { _current_position = 0; });
        else
            return;
    }
    /**
     * @brief
     * @param signal_value
     */
    template <uint8_t... PinNumbers>
    inline void light_emitting_diode_strip<PinNumbers...>::change_brightness(uint16_t signal_value)
    {
        if (get_current_position() != _range.first && get_current_position() != _range.second)
            _led_strip[get_current_position()].change_brightness(_signal_value = signal_value);
    }
    /**
     * @brief
     * @param pin_number
     * @param signal_type
     * @param signal_value
     */
    template <uint8_t... PinNumbers>
    inline void light_emitting_diode_strip<PinNumbers...>::construct(uint8_t pin_number,
                                                                     wokwi::signal_type signal_type, uint16_t signal_value)
    {
        _led_strip[_current_position++] = wokwi::light_emitting_diode{pin_number, signal_type, signal_value};
    }
    /**
     * @brief
     * @param limit
     * @param action
     */
    template <uint8_t... PinNumbers>
    template <typename Function>
    inline void light_emitting_diode_strip<PinNumbers...>::shift(uint8_t limit, Function action)
    {
        assert((limit == _range.first || limit == _range.second) && "Limits out of range");
        if (get_current_position() == limit)
            return;

        auto change_brightness = [this](uint8_t position, uint16_t signal_value)
        {
            if (position != _range.first && position != _range.second)
                _led_strip[position].change_brightness(signal_value);
        };

        change_brightness(get_current_position(), static_cast<uint16_t>(wokwi::boundary_values_of_analog_signal::low));
        action();
        change_brightness(get_current_position(), get_signal());
    }
} // namespace wokwi

#endif // !LIGHT_EMITTING_DIODE_STRIP_H