// variable_resistor.h

#ifndef VARIABLE_RESISTOR_H
#define VARIABLE_RESISTOR_H

#include "callbacks_table.h"
#include "events.h"
#include "hardware.h"
#include "helper.h"
#include "low_pass_filter.h"
#include "task.h"

#include <Arduino.h>
#include <stdint.h>

namespace wokwi
{
    template <typename LowPassFilter>
    class variable_resistor : public wokwi::electronic_component<wokwi::channel_connect_params>
    {
    public:
        using pointer_to_callback_function =
            typename wokwi::table_of_callback_functions<wokwi::variable_resistor_events>::pointer_to_callback_function;
        using table_of_pointers_to_callback_function = wokwi::table_of_callback_functions<wokwi::variable_resistor_events>;
        using event = wokwi::variable_resistor_events;
        using basic = wokwi::electronic_component<wokwi::channel_connect_params>;
        const static auto channel_number = basic::number_of_channels - 1u;

        constexpr variable_resistor() = default;
        explicit variable_resistor(uint8_t pin_number, LowPassFilter low_pass_filter = LowPassFilter{});
        variable_resistor(const variable_resistor &) = default;
        variable_resistor(variable_resistor &&) = default;
        ~variable_resistor() = default;

        variable_resistor &operator=(const variable_resistor &) = default;
        variable_resistor &operator=(variable_resistor &&) = default;
        /**
         * @brief
         * @param action
         */
        void decreased_resistance(const pointer_to_callback_function action) { _handlers.add_handler(event::decreased_resistance, action); }
        void increased_resistance(const pointer_to_callback_function action) { _handlers.add_handler(event::increased_resistance, action); }
        template <typename ValueType>
        ValueType get_signal(ValueType low, ValueType high) const;
        /**
         * @brief
         * @tparam ValueType
         * @return
         */
        template <typename ValueType>
        ValueType get_signal() const { return _signal_value; }
        uint8_t get_pin_number() const { return basic::get_pin_number(channel_number); }
        void update();

    private:
        table_of_pointers_to_callback_function _handlers{};
        LowPassFilter _low_pass_filter{};
        float _signal_value{};
    };

    /**
     * @brief
     * @param pin_number
     * @param low_pass_filter
     */
    template <typename LowPassFilter>
    inline variable_resistor<LowPassFilter>::variable_resistor(uint8_t pin_number, LowPassFilter low_pass_filter)
        : wokwi::electronic_component<wokwi::channel_connect_params>{{pin_number, wokwi::signal_type::analog, wokwi::signal_direction::input}},
          _handlers{}, _signal_value{}
    {
        analogReference(DEFAULT);
    }
    /**
     * @brief
     * @param low
     * @param high
     * @return
     */
    template <typename LowPassFilter>
    template <typename ValueType>
    inline ValueType variable_resistor<LowPassFilter>::get_signal(ValueType low, ValueType high) const
    {
        return helper::map(static_cast<ValueType>(_signal_value), static_cast<ValueType>(analog_signal_low()),
                           static_cast<ValueType>(analog_signal_high()), low, high);
    }
    /**
     * @brief
     */
    template <typename LowPassFilter>
    inline void variable_resistor<LowPassFilter>::update()
    {
        const auto signal_value = basic::read(channel_number);
        if (signal_value > _signal_value)
        {
            if (_handlers.handler(event::decreased_resistance))
                _handlers.handler(event::decreased_resistance)();
            _signal_value = signal_value;
        }

        if (signal_value < _signal_value)
        {
            if (_handlers.handler(event::increased_resistance))
                _handlers.handler(event::increased_resistance)();
            _signal_value = signal_value;
        }
    }
} // namespace wokwi
#endif // VARIABLE_RESISTOR_H