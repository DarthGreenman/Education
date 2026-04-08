// joystick.h

#ifndef JOYSTICK_H_FOR_THE_ARDUINO_UNO_PROJECT
#define JOYSTICK_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "button.h"
#include "callbacks_table.h"
#include "events.h"
#include "fox.h"
#include "variable_resistor.h"

namespace wokwi
{
    class joystick
    {
    public:
        using pointer_to_callback_function = typename wokwi::table_of_callback_functions<wokwi::joystick_events>::pointer_to_callback_function;
        using event = wokwi::joystick_events;

        enum class pin_type_element : uint8_t
        {
            horz,
            vert,
            select
        };

        constexpr joystick() = default;
        /**
         * @brief Construct a new joystick object
         *
         * @tparam Binds
         * @param binds
         */
        template <typename... Binds>
        explicit joystick(Binds &&...binds) { fox::expand_parameter_package{(construct(binds.first, binds.second), 1)...}; }
        joystick(const joystick &) = default;
        joystick(joystick &&) = default;
        ~joystick() = default;

        joystick &operator=(const joystick &) = delete;
        joystick &operator=(joystick &&) = delete;

        void right(const pointer_to_callback_function action) { _axes.horz.increased_resistance(action); }
        void left(const pointer_to_callback_function action) { _axes.horz.decreased_resistance(action); }
        void up(const pointer_to_callback_function action) { _axes.vert.decreased_resistance(action); }
        void down(const pointer_to_callback_function action) { _axes.vert.increased_resistance(action); }
        void select(const pointer_to_callback_function action) { _select.pressed(action); }
        /**
         * @brief 
         * 
         */
        void update();
        fox::pair<uint16_t, uint16_t> get_signal_axes() const { return fox::pair<uint16_t, uint16_t>{_axes.horz.get_signal_value(), _axes.vert.get_signal_value()}; }

    private:
        /**
         * @brief
         *
         * @param pin_type
         * @param pin_number
         */
        void construct(pin_type_element pin_type, uint8_t pin_number);

        struct axes
        {
            wokwi::variable_resistor horz;
            wokwi::variable_resistor vert;
        };
        axes _axes{};
        wokwi::button<> _select{};
    };

    inline void joystick::update()
    {
        _axes.horz.update();
        _axes.vert.update();
        _select.update();
    }

    inline void joystick::construct(pin_type_element pin_type, uint8_t pin_number)
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
            _select = wokwi::button<>{pin_number};
            break;
        }
    }
} // namespace wokwi

#endif // !JOYSTICK_H_FOR_THE_ARDUINO_UNO_PROJECT