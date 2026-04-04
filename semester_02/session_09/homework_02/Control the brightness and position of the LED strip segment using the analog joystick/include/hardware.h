// hardware.h

#ifndef HARDWARE_H
#define HARDWARE_H

#include "helper.h"

#include <Arduino.h>
#include <assert.h>
#include <stdint.h>

namespace wokwi
{
    enum class signal_type : uint8_t
    {
        digital,
        analog
    };
    enum class signal_direction : uint8_t
    {
        input = INPUT,
        input_pullup = INPUT_PULLUP,
        output = OUTPUT
    };
    enum class boundary_values_of_analog_signal : uint16_t
    {
        low,
        high = 1023
    };
    enum digital_signal_values : uint8_t
    {
        low,
        high
    };

    struct channel_connect_params
    {
        uint8_t pin;
        signal_type type;
        signal_direction direction;
    };

    template <typename... Channels>
    class electronic_component
    {
    protected:
        static constexpr auto number_of_channels = sizeof...(Channels);

        constexpr electronic_component() = default;
        explicit electronic_component(Channels &&...channel);
        electronic_component(const electronic_component &) = default;
        electronic_component(electronic_component &&) = default;
        virtual ~electronic_component() = default;

        electronic_component &operator=(const electronic_component &) = delete;
        electronic_component &operator=(electronic_component &&) = default;

        void write(uint8_t channel_number, wokwi::digital_signal_values signal_value) const;
        void write(uint8_t channel_number, uint16_t signal_value) const;
        uint16_t read(uint8_t channel_number) const;
        /**
         * @brief
         * @param channel_number
         * @return
         */
        uint8_t get_pin_number(uint8_t channel_number) const { return _channels[channel_number].pin; }
        /**
         * @brief
         * @param channel_number
         * @return
         */
        wokwi::signal_type get_pin_type(uint8_t channel_number) const { return _channels[channel_number].type; }

    private:
        void construct(uint8_t pin_number, wokwi::signal_type signal_type, wokwi::signal_direction signal_direction, uint8_t number);
        static bool is_valid(uint8_t pin_number, wokwi::signal_type signal_type, wokwi::signal_direction signal_direction);
        wokwi::channel_connect_params _channels[number_of_channels]{};
    };
    /**
     * @brief
     * @tparam ...Channels
     * @param ...channel
     */
    template <typename... Channels>
    inline electronic_component<Channels...>::electronic_component(Channels &&...channel)
    {
        uint8_t number{};
        helper::expand_parameter_package{(construct(channel.pin, channel.type, channel.direction, number), 1)...};
    }
    /**
     * @brief
     * @tparam ...Channels
     * @param channel_number
     * @param signal_value
     */
    template <typename... Channels>
    inline void electronic_component<Channels...>::write(uint8_t channel_number, wokwi::digital_signal_values signal_value) const
    {
        assert((signal_value == wokwi::digital_signal_values::low || signal_value == wokwi::digital_signal_values::high) &&
               "Incorrect digital signal value.");
        assert(get_pin_type(channel_number) == wokwi::signal_type::digital && "The method is designed to process digital signals.");

        digitalWrite(get_pin_number(channel_number), static_cast<uint8_t>(signal_value));
    }
    /**
     * @brief
     * @tparam ...Channels
     * @param channel_number
     * @param signal_value
     */
    template <typename... Channels>
    inline void electronic_component<Channels...>::write(uint8_t channel_number, uint16_t signal_value) const
    {
        assert(signal_value >= static_cast<uint16_t>(wokwi::boundary_values_of_analog_signal::low) &&
               signal_value <= static_cast<uint16_t>(wokwi::boundary_values_of_analog_signal::high) && "Incorrect analog signal value.");
        assert(get_pin_type(channel_number) == wokwi::signal_type::analog && "The method is designed to process analog signals.");

        analogWrite(get_pin_number(channel_number), signal_value);
    }
    /**
     * @brief
     * @tparam ...Channels
     * @param channel_number
     * @return
     */
    template <typename... Channels>
    inline uint16_t electronic_component<Channels...>::read(uint8_t channel_number) const
    {
        return get_pin_type(channel_number) == wokwi::signal_type::analog ? analogRead(get_pin_number(channel_number))
                                                                          : digitalRead(get_pin_number(channel_number));
    }
    /**
     * @brief
     * @tparam ...Channels
     * @param pin_number
     * @param signal_type
     * @param signal_direction
     */
    template <typename... Channels>
    inline void electronic_component<Channels...>::construct(uint8_t pin_number, wokwi::signal_type signal_type,
                                                             wokwi::signal_direction signal_direction, uint8_t number)
    {
        assert(is_valid(pin_number, signal_type, signal_direction) && "Incorrect channel parameters.");
        _channels[number++] = wokwi::channel_connect_params{pin_number, signal_type, signal_direction};
        pinMode(pin_number, static_cast<uint8_t>(signal_direction));
    }
    /**
     * @brief
     * @tparam ...Channels
     * @param pin_number
     * @param signal_type
     * @param signal_direction
     * @return
     */
    template <typename... Channels>
    inline bool electronic_component<Channels...>::is_valid(
        uint8_t pin_number, wokwi::signal_type signal_type, wokwi::signal_direction signal_direction)
    {
        switch (signal_type)
        {
        case wokwi::signal_type::analog:
            return signal_direction == wokwi::signal_direction::input
                       ? pin_number >= 14u && pin_number <= 19u
                       : pin_number == 3u || pin_number == 5u || pin_number == 6u || (pin_number >= 9u && pin_number <= 11u);
        case wokwi::signal_type::digital:
            return pin_number <= 13u;
        }
        return false;
    }
} // namespace wokwi

#endif // HARDWARE_H