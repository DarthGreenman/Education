// joystick.h

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "button.h"
#include "callbacks_table.h"
#include "events.h"
#include "helper.h"
#include "variable_resistor.h"

namespace wokwi
{
    template <typename LowPassFilter>
    class joystick
    {
    public:
        using pointer_to_callback_function = typename wokwi::table_of_callback_functions<wokwi::joystick_events>::pointer_to_callback_function;
        using variable_resistor = wokwi::variable_resistor<LowPassFilter>;
        using event = wokwi::joystick_events;

        enum class pin_type_element : uint8_t
        {
            horz,
            vert,
            select
        };

        constexpr joystick() = default;
        template <typename... Binds>
        explicit joystick(Binds &&...binds);
        joystick(const joystick &) = default;
        joystick(joystick &&) = default;
        ~joystick() = default;

        joystick &operator=(const joystick &) = delete;
        joystick &operator=(joystick &&) = delete;
        /**
         * @brief
         * @param action
         */
        void right(const pointer_to_callback_function action) { _axes.horz.increased_resistance(action); }
        void left(const pointer_to_callback_function action) { _axes.horz.decreased_resistance(action); }
        void up(const pointer_to_callback_function action) { _axes.vert.decreased_resistance(action); }
        void down(const pointer_to_callback_function action) { _axes.vert.increased_resistance(action); }
        void select(const pointer_to_callback_function action) { _select.pressed(action); }

        void update();
        uint8_t get_pin_number(pin_type_element pin_type) const;

    private:
        void construct(pin_type_element pin_type, uint8_t pin_number);
        struct axes
        {
            variable_resistor horz;
            variable_resistor vert;
        };
        axes _axes{};
        wokwi::button _select{};
    };
    /**
     * @brief
     * @param ...binds
     */
    template <typename LowPassFilter>
    template <typename... Binds>
    inline joystick<LowPassFilter>::joystick(Binds &&...binds)
    {
        helper::expand_parameter_package{(construct(binds.first, binds.second), 1)...};
    }
    /**
     * @brief
     */
    template <typename LowPassFilter>
    inline void joystick<LowPassFilter>::update()
    {
        _axes.horz.update();
        _axes.vert.update();
        _select.update();
    }
    /**
     * @brief
     * @param pin_type_element
     * @return
     */
    template <typename LowPassFilter>
    inline uint8_t joystick<LowPassFilter>::get_pin_number(pin_type_element pin_type) const
    {
        auto pin_number = static_cast<uint8_t>(-1);
        switch (pin_type)
        {
        case pin_type_element::horz:
            pin_number = _axes.horz.get_pin_number();
            break;
        case pin_type_element::vert:
            pin_number = _axes.vert.get_pin_number();
            break;
        case pin_type_element::select:
            pin_number = _select.get_pin_number();
            break;
        }
        return pin_number;
    }
    /**
     * @brief
     * @param pin_type
     * @param pin_number
     */
    template <typename LowPassFilter>
    inline void joystick<LowPassFilter>::construct(pin_type_element pin_type, uint8_t pin_number)
    {
        assert((pin_type == pin_type_element::horz || pin_type == pin_type_element::vert || pin_type == pin_type_element::select) &&
               "Incorrect joystick pin.");
        switch (pin_type)
        {
        case pin_type_element::horz:
            _axes.horz = variable_resistor{pin_number};
            break;
        case pin_type_element::vert:
            _axes.vert = variable_resistor{pin_number};
            break;
        case pin_type_element::select:
            _select = wokwi::button{pin_number};
            break;
        }
    }
} // namespace wokwi

#endif // JOYSTICK_H