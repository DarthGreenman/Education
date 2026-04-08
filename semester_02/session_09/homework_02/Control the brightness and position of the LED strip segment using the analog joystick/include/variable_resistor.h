// variable_resistor.h

#ifndef VARIABLE_RESISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define VARIABLE_RESISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "callbacks_table.h"
#include "events.h"
#include "hardware.h"
#include "fox.h"
#include "task.h"
#include "types_used.h"

#include <Arduino.h>
#include <stdint.h>

namespace wokwi
{
    class variable_resistor : public wokwi::electronic_component<wokwi::signal_analog, wokwi::signal_direction::input>
    {
    public:
        using pointer_to_callback_function =
            typename wokwi::table_of_callback_functions<wokwi::variable_resistor_events>::pointer_to_callback_function;
        using table_of_pointers_to_callback_function = wokwi::table_of_callback_functions<wokwi::variable_resistor_events>;
        using event = wokwi::variable_resistor_events;
        using basic = wokwi::electronic_component<wokwi::signal_analog, wokwi::signal_direction::input>;

        constexpr variable_resistor() = default;
        explicit variable_resistor(uint8_t pin_number)
            : wokwi::electronic_component<wokwi::signal_analog, wokwi::signal_direction::input>{pin_number},
              _handlers{} { analogReference(DEFAULT); }
        variable_resistor(const variable_resistor &) = default;
        variable_resistor(variable_resistor &&) = default;
        ~variable_resistor() = default;

        variable_resistor &operator=(const variable_resistor &) = default;
        variable_resistor &operator=(variable_resistor &&) = default;

        void decreased_resistance(const pointer_to_callback_function action) { _handlers.add_handler(event::decreased_resistance, action); }
        void increased_resistance(const pointer_to_callback_function action) { _handlers.add_handler(event::increased_resistance, action); }

        template <typename T>
        T get_signal_value(T low, T high) const;
        uint16_t get_signal_value() const { return basic::read(); }
        /**
         * @brief 
         * 
         */
        void update();

    private:
        table_of_pointers_to_callback_function _handlers{};
    };

    template <typename T>
    inline T variable_resistor::get_signal_value(T low, T high) const
    {
        const auto signal_value = get_signal_value();
        return fox::map(static_cast<T>(signal_value), static_cast<T>(wokwi::signal_analog::low),
                           static_cast<T>(wokwi::signal_analog::high), low, high);
    }

    inline void variable_resistor::update()
    {
        static auto prev_signal_value = static_cast<uint16_t>(-1);
        const auto signal_value = basic::read();
        if (signal_value > prev_signal_value)
        {
            if (_handlers.handler(event::decreased_resistance))
                _handlers.handler(event::decreased_resistance)();
            prev_signal_value = signal_value;
        }

        if (signal_value < prev_signal_value)
        {
            if (_handlers.handler(event::increased_resistance))
                _handlers.handler(event::increased_resistance)();
            prev_signal_value = signal_value;
        }
    }
} // namespace wokwi
#endif // VARIABLE_RESISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT